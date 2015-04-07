import sys

from jpyutils import task_runner

if __name__ == '__main__':
    render_arguments = dict()

    render_arguments["LOCAL_ROOT"] = "../"
    render_arguments["HADOOP_BIN"] = "/home/zhangjian09/software/hadoop-client/hadoop/bin/hadoop"
    render_arguments["DATE"] = "2015-03-10"
    render_arguments["REF_DATE"] = "2015-03-18"
    render_arguments["HDFS_JOINED_LOG_DIR"] = "/app/ecom/fcr-opt/kr/zhangjian09/2015/data/join_kr_log"
    render_arguments["HDFS_ORIGIN_LOG_DIR"] = "/app/ecom/fcr-opt/kr/analytics"

    t = task_runner.Task("../log", render_arguments, parallel_degree=4)
    t.add_job_from_file("../conf/test.jobconf", "gbk")
    #t.list_jobs()
    t.run()
