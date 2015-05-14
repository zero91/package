import sys

from jpyutils import task_runner

def test_add_job():
    task = task_runner.Task("../log");
    task.add_job("test001", "sleep 2", comment="sleep", shell=True);
    task.add_job("test002", "sleep 1", comment="sleep2", depends="test001", shell=True);
    task.run();

def test_add_jobs_from_file():
    render_arguments = dict()

    render_arguments["LOCAL_ROOT"] = "../"
    render_arguments["HADOOP_BIN"] = "/home/zhangjian09/software/hadoop-client/hadoop/bin/hadoop"
    render_arguments["DATE"] = "2015-03-10"
    render_arguments["REF_DATE"] = "2015-03-18"
    render_arguments["HDFS_JOINED_LOG_DIR"] = "/app/ecom/fcr-opt/kr/zhangjian09/2015/data/join_kr_log"
    render_arguments["HDFS_ORIGIN_LOG_DIR"] = "/app/ecom/fcr-opt/kr/analytics"

    t = task_runner.Task("../log", render_arguments, parallel_degree=4)
    t.add_jobs_from_file("../conf/test.jobconf", "gbk")
    #t.list_jobs()
    t.run()

if __name__ == '__main__':
    test_add_job()
    test_add_jobs_from_file()

