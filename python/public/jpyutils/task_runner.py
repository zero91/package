# Author: Donald Cheung <jianzhang9102@gmail.com>
''' Tools for doing task which contains a chain of jobs '''
import sys
import os
import subprocess
import threading
import signal
import time
import re
from datetime import datetime, timedelta

from jpyutils import utils

__all__ = ['Job', 'Task']

JOB_STATUS_NOTSTARTED          = 1
JOB_STATUS_RUNNING             = 2
JOB_STATUS_DONE                = 3
JOB_STATUS_FAILED              = 4
JOB_STATUS_KILLED              = 5
JOB_STATUS_CHECK_BEFORE_FAILED = 6
JOB_STATUS_CHECK_AFTER_FAILED  = 7

job_status_desc = {
    JOB_STATUS_NOTSTARTED          : ("Not Started", "brown"),
    JOB_STATUS_RUNNING             : ("Running",     "cyan"),
    JOB_STATUS_DONE                : ("Done",        "green"),
    JOB_STATUS_FAILED              : ("Failed",      "red"),
    JOB_STATUS_KILLED              : ("Killed",      "purple"),
    JOB_STATUS_CHECK_BEFORE_FAILED : ("Check_before Failed", "red"),
    JOB_STATUS_CHECK_AFTER_FAILED  : ("Check_after Failed",  "red")
}

class Job(threading.Thread):
    ''' A single job class '''
    def __init__(self, log_dir, jobname, command, check_before="",
                                                  check_after="",
                                                  depends="",
                                                  comment=""):
        threading.Thread.__init__(self, name=jobname)
        self.__log_dir = log_dir

        if len(depends.strip()) == 0:
            self.__depend_set = set()
        else:
            self.__depend_set = set([depend.strip() for depend in depends.split(',')])
        self.__depend_done_set = set()
        self.__reverse_depend_set = set()

        self.__jobname = jobname
        self.__comment = comment
        self.__status = JOB_STATUS_NOTSTARTED

        self.__start_time = datetime(1900, 1, 1)
        self.__end_time = datetime(1900, 1, 1)

        os.system("mkdir -p %s" % self.__log_dir)
        if os.path.isdir(self.__log_dir):
            self.__stdout = open("%s/%s.stdout" % (self.__log_dir, self.__jobname), "w")
            self.__stderr = open("%s/%s.stderr" % (self.__log_dir, self.__jobname), "w")
        else:
            self.__stdout = subprocess.PIPE
            self.__stderr = subprocess.PIPE

        self.__check_before = check_before
        self.__check_before_started = False
        self.__check_before_process = None
        self.__check_before_returncode = None

        self.__check_after = check_after
        self.__check_after_started = False
        self.__check_after_process = None
        self.__check_after_returncode = None

        self.__command = command
        self.__run_main_started = False
        self.__run_main_process = None
        self.__run_main_returncode = None

        self.__current_process = None
        self.returncode = None

    def get_jobname(self):
        return self.__jobname

    def get_command(self):
        return self.__command
        
    def get_depends(self):
        return self.__depend_set

    def get_comment(self):
        return self.__comment

    def get_status(self):
        return self.__status

    def add_reverse_depend(self, jobname):
        self.__reverse_depend_set.add(jobname)

    def get_reverse_depend(self):
        return self.__reverse_depend_set

    def __str__(self):
        if self.__start_time > self.__end_time:
            elapse_time = datetime.now() - self.__start_time
        else:
            elapse_time = self.__end_time - self.__start_time

        elapse_float = elapse_time.total_seconds() + float(elapse_time.microseconds / 10000) * 0.01

        format_str = "%s" % self.__jobname.ljust(30)
        format_str += " | " + utils.color_str(
                                    "%s" % (job_status_desc[self.__status][0].ljust(20)),
                                    job_status_desc[self.__status][1])
        format_str += " | " + (" %.2fs" % elapse_float).ljust(20)
        format_str += " | " + self.__start_time.strftime("%Y-%m-%d %H-%M-%S").ljust(20)
        format_str += " | " + self.__end_time.strftime("%Y-%m-%d %H-%M-%S").ljust(20)

        format_str = format_str.ljust(130)
        return format_str

    def __run_check_before(self):
        if len(self.__check_before.strip()) == 0:
            self.__check_before_returncode = 0
        elif not self.__check_before_started:
            self.__check_before_process = subprocess.Popen(self.__check_before,
                                                           stdin=subprocess.PIPE,
                                                           stderr=self.__stderr,
                                                           stdout=self.__stdout,
                                                           shell=True,
                                                           close_fds=True,
                                                           preexec_fn=os.setsid)
            self.__current_process = self.__check_before_process
            self.__check_before_returncode = self.__check_before_process.poll()
            self.__check_before_started = True
        else:
            self.__check_before_returncode = self.__check_before_process.poll()
        return self.__check_before_returncode

    def __run_main(self):
        if not self.__run_main_started:
            self.__run_main_process = subprocess.Popen(self.__command,
                                                       stdin=subprocess.PIPE,
                                                       stderr=self.__stderr,
                                                       stdout=self.__stdout,
                                                       shell=True,
                                                       close_fds=True,
                                                       preexec_fn=os.setsid)
            self.__current_process = self.__run_main_process
            self.__run_main_returncode = self.__run_main_process.poll()
            self.__run_main_started = True
        else:
            self.__run_main_returncode = self.__run_main_process.poll()
        return self.__run_main_returncode

    def __run_check_after(self):
        if len(self.__check_after.strip()) == 0:
            self.__check_after_returncode = 0
        elif not self.__check_after_started:
            self.__check_after_process = subprocess.Popen(self.__check_after,
                                                          stdin=subprocess.PIPE,
                                                          stderr=self.__stderr,
                                                          stdout=self.__stdout,
                                                          shell=True,
                                                          close_fds=True,
                                                          preexec_fn=os.setsid)
            self.__current_process = self.__check_after_process
            self.__check_after_returncode = self.__check_after_process.poll()
            self.__check_after_started = True
        else:
            self.__check_after_returncode = self.__check_after_process.poll()
        return self.__check_after_returncode

    def is_ready(self):
        return len(self.__depend_set) == len(self.__depend_done_set)

    def run(self):
        if not self.is_ready():
            sys.stderr.write("Job [%s] not ready.\n" % self.__jobname)
            return

        self.__start_time = datetime.now()
        self.__status = JOB_STATUS_RUNNING

        while self.__run_check_before() == None:
            time.sleep(0.1)
        if self.__check_before_returncode != 0:
            sys.stderr.write("The check_before failed for job [%s].\n" % self.__jobname)
            self.returncode = self.__check_before_returncode
            self.__status = JOB_STATUS_CHECK_BEFORE_FAILED
            return

        while self.__run_main() == None:
            time.sleep(0.1)
        if self.__run_main_returncode != 0:
            sys.stderr.write("Command execution failed for job [%s].\n" % self.__jobname)
            self.returncode = self.__run_main_returncode
            self.__status = JOB_STATUS_FAILED
            return

        while self.__run_check_after() == None:
            time.sleep(0.1)
        if self.__run_main_returncode != 0:
            sys.stderr.write("The check_after failed for job [%s].\n" % self.__jobname)
            self.returncode = self.__check_after_returncode
            self.__status = JOB_STATUS_CHECK_AFTER_FAILED
            return

        self.returncode = 0
        self.__status = JOB_STATUS_DONE
        self.__end_time = datetime.now()

    def suicide(self):
        if self.__current_process != None and self.__current_process.poll() == None:
            #self.__current_process.kill()
            os.killpg(self.__current_process.pid, signal.SIGTERM)
            #os.kill(self.__current_process.pid, signal.SIGTERM)
            self.__status = JOB_STATUS_KILLED
        return True

    def notified_done(self, done_jobname):
        if done_jobname in self.__depend_set:
            self.__depend_done_set.add(done_jobname)

    def dumps(self):
        dump_str = 'JOB('
        dump_str +=    'jobname="%s",' % (self.__jobname)
        dump_str +=    'command="%s",' % (self.__command)
        dump_str +=    'check_before="%s",' % (self.__check_before)
        dump_str +=    'check_after="%s",' % (self.__check_after)
        dump_str +=    'depends="%s",' % (",".join(self.__depend_set))
        dump_str +=    'comment="%s",' % (self.__comment)
        dump_str += ')';
        return dump_str

class Task:
    ''' A task manager class '''
    def __init__(self, log_dir, render_arguments={}, parallel_degree=sys.maxint):
        self.__job_dict = dict()
        self.__has_check = False
        self.__render_arguments = render_arguments

        self.__job_ready_set = set()
        self.__running_jobs = set()
        self.__sorted_jobs = list()

        self.__parallel_degree = parallel_degree
        self.__log_dir = log_dir

        self.__started = False

    def add_job(self, job_info):
        if "jobname" not in job_info:
            sys.stderr.write("[ERROR] New Job has no jobname.\n")
            return False
        if "command" not in job_info:
            sys.stderr.write("[ERROR] New Job has no command.\n")
            return False

        jobname = job_info['jobname']
        command = job_info['command']

        check_before = ""
        if check_before in job_info:
            check_before = job_info["check_before"]

        check_after = ""
        if check_after in job_info:
            check_after = job_info["check_after"]

        depends = ""
        if depends in job_info:
            depends = job_info["depends"]

        comment = ""
        if comment in job_info:
            comment = job_info["comment"]
        return self.__add_one_job(jobname, command, check_before, check_after, depends, comment)

    def add_job_from_str(self, jobs_str, encoding="utf-8"):
        self.__has_check = False
        self.__load_from_str(jobs_str, encoding)

    def add_job_from_file(self, jobs_fname, encoding="utf-8"):
        self.__has_check = False
        self.__load_from_str(open(jobs_fname, 'r').read(), encoding)

    def check(self):
        for jobname, job in self.__job_dict.iteritems():
            for depend_jobname in job.get_depends():
                if depend_jobname not in self.__job_dict:
                    return False
                self.__job_dict[depend_jobname].add_reverse_depend(jobname)
        self.__has_check = True
        return True

    def __next(self, remain_job_set):
        next_job_set = set()
        for jobname in remain_job_set:
            if self.__job_dict[jobname].is_ready():
                next_job_set.add(jobname)
        remain_job_set ^= next_job_set
        return next_job_set

    def run(self):
        if self.__started:
            sys.stderr.write("Task should be executed only once")
            return False

        if self.__sort_jobs() == False:
            return False

        if not self.__has_check and not self.check():
            return False

        self.__started = True
        signal.signal(signal.SIGINT, self.__kill_sig_handler)
        signal.signal(signal.SIGTERM, self.__kill_sig_handler)

        remain_job_set = set(self.__job_dict)
        self.__display_job_status()
        while True:
            if len(self.__running_jobs) < self.__parallel_degree:
                self.__job_ready_set |= self.__next(remain_job_set)

            while len(self.__running_jobs) < self.__parallel_degree \
                    and len(self.__job_ready_set) > 0:
                new_jobname = self.__job_ready_set.pop()
                self.__job_dict[new_jobname].start()
                self.__running_jobs.add(new_jobname)

            finished_job_set = set()
            for jobname in self.__running_jobs:
                if self.__job_dict[jobname].is_alive():
                    continue
                finished_job_set.add(jobname)
                if self.__job_dict[jobname].returncode != 0:
                    sys.stderr.write("The job [%s] failed, return code [%d]\n" % (
                                            self.__job_dict[jobname].get_jobname(),
                                            self.__job_dict[jobname].returncode))
                    self.__kill_all_processes()
                    self.__display_job_status(recovery=True)
                    return False
            self.__running_jobs ^= finished_job_set

            for jobname in finished_job_set:
                for reverse_depend_jobname in self.__job_dict[jobname].get_reverse_depend():
                    self.__job_dict[reverse_depend_jobname].notified_done(jobname)

            self.__display_job_status(recovery=True)
            if len(remain_job_set) == 0 and len(self.__running_jobs) == 0:
                return True
            time.sleep(0.1)

    def list_jobs(self):
        if self.__sort_jobs() == False:
            return False
        self.__display_job_status()

    def __kill_sig_handler(self, signum, frame):
        self.__kill_all_processes()
        self.__display_job_status()
        sys.stderr.write("\nSignal [%d] received. All running processes are killed.\n" % signum)
        exit(-1)

    def __add_one_job(self, jobname, command, check_before="",
                                              check_after="",
                                              depends="",
                                              comment=""):
        if len(jobname.strip()) == 0:
            sys.stderr.write("[ERROR] jobname is empty.\n")
            return False
        job = Job(self.__log_dir, jobname.strip(),
                                  command.strip(),
                                  check_before.strip(),
                                  check_after.strip(),
                                  depends.strip(),
                                  comment.strip())
        self.__job_dict[jobname.strip()] = job
        self.__check = False
        return True

    def __lookup_map(self, reg_match):
        return self.__render_arguments[reg_match.group(1).strip()]

    def __load_from_str(self, jobs_str, encoding="utf-8"):
        jobs_str = jobs_str.decode(encoding)
        render_arg_pattern = re.compile(r"\{\%(.*?)\%\}")
        all_match_str = re.findall(render_arg_pattern, jobs_str)
        for match_str in all_match_str:
            if match_str not in self.__render_arguments:
                sys.stderr.write("[ERROR] Missing value for render argument [%s].\n" % match_str)
                continue
        jobs_str = render_arg_pattern.sub(self.__lookup_map, jobs_str)
        exec(jobs_str, {}, {'JOB': self.__add_one_job})

    def __kill_all_processes(self):
        for jobname in self.__running_jobs:
            self.__job_dict[jobname].suicide()
        self.__running_jobs = set()

    def __sort_jobs(self):
        if not self.check():
            return False
        self.__sorted_jobs = list()
        indegree_dict = dict()
        for jobname, job in self.__job_dict.iteritems():
            indegree_dict[jobname] = len(job.get_depends())

        while len(self.__sorted_jobs) < len(self.__job_dict):
            new_list = [jobname for jobname in indegree_dict if indegree_dict[jobname] == 0]
            for jobname in new_list:
                indegree_dict.pop(jobname)
                for reverse_depend_jobname in self.__job_dict[jobname].get_reverse_depend():
                    indegree_dict[reverse_depend_jobname] -= 1
            if len(new_list) == 0:
                sys.stderr.write("[ERROR] Not a DAG job graph, there may exist a circle.\n")
                return False
            self.__sorted_jobs.extend(new_list)
        return True

    def __display_job_status(self, recovery=False):
        if recovery:
            sys.stderr.write("\033[%dA" % (len(self.__sorted_jobs) * 2 + 1))

        sort_id = 0
        for jobname in self.__sorted_jobs:
            sort_id += 1
            sys.stderr.write("%s\n" % ('-' * 130))
            sys.stderr.write("%s%s\n" % (("[%d]." % sort_id).ljust(6), self.__job_dict[jobname]))
        sys.stderr.write("%s\n" % ('-' * 130))

