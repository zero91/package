#coding: utf-8
import sys
import os

from jpyutils import task_runner

if __name__ == '__main__':
    task = task_runner.Task("../../log")

    task.add_job("calc_sim_user", "python preprocess.py -i ../../data_in/train_data.txt -o ../../data_out/cf/cf", comment = "计算用户间相似列表")
    task.add_job("calc_result", "python calc_result.py -i ../../data_out/cf/cf", comment = "计算用户间相似列表", depends = "calc_sim_user")

    task.run()

