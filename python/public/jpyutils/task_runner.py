# Author: Donald Cheung <jianzhang9102@gmail.com>
'''
Tools for doing task which contains a chain of jobs, and can executing jobs in parallel.
'''

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

_JOB_STATUS_NOTSTARTED          = 1
_JOB_STATUS_RUNNING             = 2
_JOB_STATUS_DONE                = 3
_JOB_STATUS_FAILED              = 4
_JOB_STATUS_KILLED              = 5
_JOB_STATUS_CHECK_BEFORE_FAILED = 6
_JOB_STATUS_CHECK_AFTER_FAILED  = 7

_job_status_desc = {
    _JOB_STATUS_NOTSTARTED          : ("Not Started", "brown"),
    _JOB_STATUS_RUNNING             : ("Running",     "cyan"),
    _JOB_STATUS_DONE                : ("Done",        "green"),
    _JOB_STATUS_FAILED              : ("Failed",      "red"),
    _JOB_STATUS_KILLED              : ("Killed",      "purple"),
    _JOB_STATUS_CHECK_BEFORE_FAILED : ("Check_before Failed", "red"),
    _JOB_STATUS_CHECK_AFTER_FAILED  : ("Check_after Failed",  "red")
}

class Job(threading.Thread):
    '''Maintaining a single job.

    It maintains a single job, and can be used to run the job, get the result of it.

    Parameters
    ----------
    jobname: str
        The name of the job. Best using naming method of programming languages,
        for it will be used to create log files on disk.

    command: str
        The command to be executed of the job.

    check_before: str
        The command to be executed before the job. Default None.

    check_after: str
        The command to be executed after the job. Default None.

    depend: str
        A string which is the concatenation of the jobnames of all the jobs which must be
        executed before this job. Separated by a single comma(',').

    comment: str
        Comment string on the job, mainly for user to understand what this job is about.

    log_dir: str
        The directory which used to save log files of this job. Default to be None,
        which will not output any data.  If set to be empty string(""),
        sys.stdout and sys.stderr will be used.

    shell: boolean
        The shell argument (which defaults to False) specifies whether to use the shell
        as the program to execute. If shell is True, it is recommended to pass args as
        a string rather than as a sequence.

    Notes
    -----
        When log_dir is set to be None, Job will not output any data. Internally, Job will use
        subprocess.PIPE, which may cause dead LOCK when output data is large.
    '''
    def __init__(self, jobname, command, check_before = None,
                                         check_after  = None,
                                         depends      = None,
                                         comment      = None,
                                         log_dir      = None,
                                         shell        = True):
        threading.Thread.__init__(self, name=jobname.strip())
        self.__jobname = jobname.strip()

        self.__command = command
        self.__run_main_started = False
        self.__run_main_process = None
        self.__run_main_returncode = None

        self.__check_before = check_before
        self.__check_before_started = False
        self.__check_before_process = None
        self.__check_before_returncode = None

        self.__check_after = check_after
        self.__check_after_started = False
        self.__check_after_process = None
        self.__check_after_returncode = None

        if depends == None:
            self.__depend_set = set()
        else:
            self.__depend_set = set([depend.strip() for depend in depends.split(',')])
        self.__depend_done_set = set()
        self.__reverse_depend_set = set()

        self.__comment = comment

        self.__log_dir = log_dir
        if self.__log_dir == None:
            self.__stdout = subprocess.PIPE
            self.__stderr = subprocess.PIPE
        else:
            self.__log_dir = self.__log_dir.strip()
            if self.__log_dir != "":
                os.system("mkdir -p %s" % self.__log_dir)
                if os.path.isdir(self.__log_dir):
                    self.__stdout = open("%s/%s.stdout" % (self.__log_dir, self.__jobname), "w")
                    self.__stderr = open("%s/%s.stderr" % (self.__log_dir, self.__jobname), "w")
                else:
                    self.__stdout = sys.stdout
                    self.__stderr = sys.stderr
            else:
                self.__stdout = sys.stdout
                self.__stderr = sys.stderr

        self.__shell = shell
        self.__current_process = None
        self.__status = _JOB_STATUS_NOTSTARTED
        self.__start_time = datetime(1900, 1, 1)
        self.__end_time = datetime(1900, 1, 1)
        self.returncode = None

    def __getitem__(self, key):
        if key == "jobname":
            return self.__jobname
        if key == "command":
            return self.__command
        if key == "depends":
            return self.__depend_set
        if key == "comment":
            return self.__comment
        if key == "status":
            return self.__status
        if key == "reverse_depend":
            return self.__reverse_depend_set
        sys.stderr.write("[%s]: [%s] not found\n" % (utils.color_str("ERROR"), key))
        return None

    def add_reverse_depend(self, jobname):
        '''Add a job which depends on this job

        This reverse depend info will be used when this job is done, and should tell
        reverse depend jobs the info.

        Parameters
        ----------
        jobname: str
            The name of the job to be reverse depends.
        '''
        self.__reverse_depend_set.add(jobname)

    def __str__(self):
        if self.__start_time > self.__end_time:
            elapse_time = datetime.now() - self.__start_time
        else:
            elapse_time = self.__end_time - self.__start_time

        elapse_float = elapse_time.total_seconds() + float(elapse_time.microseconds / 10000) * 0.01

        format_str = "%s" % self.__jobname.ljust(30)
        format_str += " | " + utils.color_str(
                                    "%s" % (_job_status_desc[self.__status][0].ljust(12)),
                                    _job_status_desc[self.__status][1])
        format_str += " | " + (" %.2fs" % elapse_float).ljust(10)
        format_str += " | " + self.__start_time.strftime("%Y-%m-%d %H:%M:%S").ljust(20)
        format_str += " | " + self.__end_time.strftime("%Y-%m-%d %H:%M:%S").ljust(20)
        if self.__comment != None:
            format_str += " | " + self.__comment.ljust(20)
        format_str = format_str.ljust(130)
        return format_str

    def __run_check_before(self):
        if self.__check_before == None:
            self.__check_before_returncode = 0
        elif not self.__check_before_started:
            self.__check_before_process = subprocess.Popen(self.__check_before,
                                                           stdin=subprocess.PIPE,
                                                           stderr=self.__stderr,
                                                           stdout=self.__stdout,
                                                           shell=self.__shell,
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
                                                       shell=self.__shell,
                                                       close_fds=True,
                                                       preexec_fn=os.setsid)
            self.__current_process = self.__run_main_process
            self.__run_main_returncode = self.__run_main_process.poll()
            self.__run_main_started = True
        else:
            self.__run_main_returncode = self.__run_main_process.poll()
        return self.__run_main_returncode

    def __run_check_after(self):
        if self.__check_after == None:
            self.__check_after_returncode = 0
        elif not self.__check_after_started:
            self.__check_after_process = subprocess.Popen(self.__check_after,
                                                          stdin=subprocess.PIPE,
                                                          stderr=self.__stderr,
                                                          stdout=self.__stdout,
                                                          shell=self.__shell,
                                                          close_fds=True,
                                                          preexec_fn=os.setsid)
            self.__current_process = self.__check_after_process
            self.__check_after_returncode = self.__check_after_process.poll()
            self.__check_after_started = True
        else:
            self.__check_after_returncode = self.__check_after_process.poll()
        return self.__check_after_returncode

    def is_ready(self):
        """Return whether the job is ready of not

        If all the jobs which this job depend on have been done, return true.
        Otherwise return false.
        """
        return len(self.__depend_set) == len(self.__depend_done_set)

    def run(self):
        """Start running this job.

        Should be only executed once. Behavior of more than one execution does not defined.
        """
        if not self.is_ready():
            sys.stderr.write("Job [%s] not ready.\n" % self.__jobname)
            return

        self.__start_time = datetime.now()
        self.__status = _JOB_STATUS_RUNNING

        while self.__run_check_before() == None:
            time.sleep(0.1)
        if self.__check_before_returncode != 0:
            sys.stderr.write("The check_before failed for job [%s].\n" % self.__jobname)
            self.returncode = self.__check_before_returncode
            self.__status = _JOB_STATUS_CHECK_BEFORE_FAILED
            return

        while self.__run_main() == None:
            time.sleep(0.1)
        if self.__run_main_returncode != 0:
            sys.stderr.write("Command execution failed for job [%s].\n" % self.__jobname)
            self.returncode = self.__run_main_returncode
            self.__status = _JOB_STATUS_FAILED
            return

        while self.__run_check_after() == None:
            time.sleep(0.1)
        if self.__run_main_returncode != 0:
            sys.stderr.write("The check_after failed for job [%s].\n" % self.__jobname)
            self.returncode = self.__check_after_returncode
            self.__status = _JOB_STATUS_CHECK_AFTER_FAILED
            return

        self.returncode = 0
        self.__status = _JOB_STATUS_DONE
        self.__end_time = datetime.now()

    def suicide(self):
        """Suicide this job
        """
        if self.__current_process != None and self.__current_process.poll() == None:
            self.__current_process.kill()
            #os.killpg(self.__current_process.pid, signal.SIGTERM)
            #os.kill(self.__current_process.pid, signal.SIGTERM)
            self.__status = _JOB_STATUS_KILLED
        return True

    def notified_done(self, done_jobname):
        '''Add a job which depends on this job

        This reverse depend info will be used when this job is done, and should tell
        reverse depend jobs the info.

        Parameters
        ----------
        jobname: str
            The name of the job to be reverse depended.
        '''
        if done_jobname in self.__depend_set:
            self.__depend_done_set.add(done_jobname)

    def dump_str(self):
        '''Dump this job into a string

        The string can be used to recreate this job latter.
        '''
        dump = 'JOB('
        dump +=     'jobname="%s",' % (self.__jobname)
        dump +=     'command="%s",' % (self.__command)

        if self.__check_before != None:
            dump += 'check_before="%s",' % (self.__check_before)
        if self.__check_after != None:
            dump += 'check_after="%s",' % (self.__check_after)
        if len(self.__depend_set) > 0:
            dump += 'depends="%s",' % (",".join(self.__depend_set))
        if self.__comment != None:
            dump += 'comment="%s",' % (self.__comment)
        if self.__log_dir != None:
            dump += 'log_dir="%s",' % (self.__log_dir)

        dump +=     'shell="%s",' % (self.__shell)
        dump += ')';
        return dump

class Task:
    ''' A task manage class

    It maintains a batch of jobs, and run the jobs according to their topological relations.

    Parameters
    ---------
    log_dir: str
        The directory which used to save log files of this task. Default to be None,
        which will not output any data.  If set to be empty string(""),
        sys.stdout and sys.stderr will be used.

    render_arguments: dict
        Dict which used to replace job string's parameter for its true value.

    parallel_degree: int
        Parallel degree of this task. Jobs will be run at most this number at the same time.

    Notes
    -----
        When log_dir is set to be None, Job will not output any data. Internally, Job will use
        subprocess.PIPE, which may cause dead LOCK when output data is large.
    '''
    def __init__(self, log_dir=None, render_arguments={}, parallel_degree=sys.maxint):
        self.__log_dir = log_dir
        self.__render_arguments = render_arguments
        self.__parallel_degree = parallel_degree

        self.__job_dict = dict()
        self.__running_jobs = set()
        self.__sorted_jobs = list()

        self.__has_check = False
        self.__started = False

    def add_job(self, jobname, command, check_before = None,
                                        check_after  = None,
                                        depends      = None,
                                        comment      = None,
                                        shell        = True):
        '''Add a new job to this task

        Parameters
        ----------
        jobname: str
            The name of the job. Best using naming method of programming languages,
            for it will be used to create log files on disk.

        command: str
            The command to be executed of the job.

        check_before: str
            The command to be executed before the job. Default None.

        check_after: str
            The command to be executed after the job. Default None.

        depend: str
            A string which is the concatenation of the jobnames of all the jobs which must be
            executed before this job. Separated by a single comma(',').

        comment: str
            Comment string on the job, mainly for user to understand what this job is about.

        shell: boolean
            The shell argument (which defaults to False) specifies whether to use the shell
            as the program to execute. If shell is True, it is recommended to pass args as
            a string rather than as a sequence.
        '''
        job = Job(jobname, command, check_before = check_before,
                                    check_after  = check_after,
                                    depends      = depends,
                                    comment      = comment,
                                    log_dir      = self.__log_dir,
                                    shell        = shell)
        if job["jobname"] in self.__job_dict:
            sys.stderr.write("Task is already has the same job name of this job, check it!\n")
            return False
        self.__job_dict[job["jobname"]] = job
        self.__has_check = False
        return True

    def add_jobs_from_str(self, jobs_str, encoding="utf-8"):
        '''Add a new job which dumps in str to this task.

        Parameters
        ----------
        jobs_str: str
            The string of a job
        '''
        jobs_str = jobs_str.decode(encoding)
        render_arg_pattern = re.compile(r"\<\%=(.*?)\%\>")
        all_match_str = re.findall(render_arg_pattern, jobs_str)
        for match_str in all_match_str:
            if match_str not in self.__render_arguments:
                sys.stderr.write("[ERROR] Missing value for render argument [%s].\n" % match_str)
                continue

        def __lookup_func(self, reg_match):
            return self.__render_arguments[reg_match.group(1).strip()]
        jobs_str = render_arg_pattern.sub(__lookup_func, jobs_str)
        exec(jobs_str, {}, {'JOB': self.add_job})
        return True

    def add_jobs_from_file(self, jobs_fname, encoding="utf-8"):
        '''Add jobs from a file

        Parameters
        ----------
        jobs_fname: str
            The file name of which contains jobs string
        '''
        return self.add_jobs_from_str(open(jobs_fname, 'r').read(), encoding)

    def __check(self):
        for jobname, job in self.__job_dict.iteritems():
            for depend_jobname in job["depends"]:
                if depend_jobname not in self.__job_dict:
                    sys.stderr.write("Job %s's depend job %s does not exist\n" % ( \
                                                        jobname, depend_jobname))
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
        '''Running all jobs of this task.

        Notes
        ----
            Should only be executed once.
        '''
        if self.__started:
            sys.stderr.write("Task should be executed only once")
            return False
        if not self.__has_check and not self.__check():
            return False
        if not self.__sort_jobs():
            return False

        signal.signal(signal.SIGINT, self.__kill_signal_handler)
        signal.signal(signal.SIGTERM, self.__kill_signal_handler)

        self.__display_job_status()
        self.__started = True
        job_ready_set = set()
        remain_job_set = set(self.__job_dict)
        while True:
            if len(self.__running_jobs) < self.__parallel_degree:
                job_ready_set |= self.__next(remain_job_set)

            while len(self.__running_jobs) < self.__parallel_degree \
                    and len(job_ready_set) > 0:
                new_jobname = job_ready_set.pop()
                self.__job_dict[new_jobname].start()
                self.__running_jobs.add(new_jobname)

            finished_job_set = set()
            for jobname in self.__running_jobs:
                if self.__job_dict[jobname].is_alive():
                    continue
                finished_job_set.add(jobname)
                if self.__job_dict[jobname].returncode != 0:
                    sys.stderr.write("Job [%s] failed, return code [%d]\n" % (
                                            self.__job_dict[jobname]["jobname"],
                                            self.__job_dict[jobname].returncode))
                    self.__kill_all_processes()
                    self.__display_job_status(recovery=True)
                    return False
            self.__running_jobs ^= finished_job_set

            for jobname in finished_job_set:
                for reverse_depend_jobname in self.__job_dict[jobname]["reverse_depend"]:
                    self.__job_dict[reverse_depend_jobname].notified_done(jobname)

            self.__display_job_status(recovery=True)
            if len(remain_job_set) == 0 and len(self.__running_jobs) == 0:
                return True
            time.sleep(0.1)

    def __kill_signal_handler(self, sig_num, stack):
        self.__kill_all_processes()
        self.__display_job_status()
        sys.stderr.write("\nSignal [%d] received. All running processes are killed.\n" % sig_num)
        exit(-1)

    def __kill_all_processes(self):
        for jobname in self.__running_jobs:
            self.__job_dict[jobname].suicide()
        self.__running_jobs = set()

    def __sort_jobs(self):
        if not self.__check():
            return False

        self.__sorted_jobs = list()
        indegree_dict = dict()
        for jobname, job in self.__job_dict.iteritems():
            indegree_dict[jobname] = len(job["depends"])

        while len(self.__sorted_jobs) < len(self.__job_dict):
            new_list = [jobname for jobname in indegree_dict if indegree_dict[jobname] == 0]
            for jobname in new_list:
                indegree_dict.pop(jobname)
                for reverse_depend_jobname in self.__job_dict[jobname]["reverse_depend"]:
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

    def list_jobs(self):
        '''List all jobs of this task
        '''
        if not self.__sort_jobs():
            return False
        self.__display_job_status()
