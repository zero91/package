#coding: utf-8
import sys
import os

from argparse import ArgumentParser
from jpyutils import utils

def read_user_sim(user_sim_fname):
    userid = 0
    user_sim_dict = dict()
    for line in open(user_sim_fname, 'r').xreadlines():
        line = line.strip()
        if line == '':
            continue
        userid, sim_user_list = line.split('\t')
        user_sim_dict.setdefault(userid,
                                [userid_sim.split(':') for userid_sim in sim_user_list.split(',')])
    return user_sim_dict

def read_user2news(user2news_fname):
    user2news_dict = dict()
    for line in open(user2news_fname, 'r').xreadlines():
        line = line.strip()
        if line == '':
            continue
        userid, news_list = line.split('\t')
        user2news_dict.setdefault(userid, set())
        user2news_dict[userid] |= set(news_list.split(','))
    return user2news_dict

def calc_result(user_sim_dict, user2news, result_fname):
    result_file = open(result_fname, "w")
    result_file.write("userid,newsid\n")
    for userid, sim_list in user_sim_dict.iteritems():
        if len(sim_list) == 0:
            continue
        sim_user = sim_list[0][0]

        news_recommend_list = user2news[sim_user] - user2news[userid]
        if len(news_recommend_list) == 0:
            continue
        news_id = news_recommend_list.pop()

        result_file.write("%s,%s\n" % (userid, news_id))
        #result_file.write("%s\n" % ("\n".join(["%s,%s" % (userid, news_id) \
        #                            for news_id in news_recommend_list])))

    result_file.close()

def parse_args():
    arg_parser = ArgumentParser()
    arg_parser.add_argument("-i", "--input", required=True, help="The prefix of preprocess file name")
    arg_parser.add_argument("-o", "--output", help="The final submission file")
    return arg_parser.parse_args()

if __name__ == '__main__':
    args = parse_args()

    sys.stderr.write("Loading data...")
    user_sim_dict = read_user_sim("%s.user_sim" % (args.input))
    user2news = read_user2news("%s.user2news" % (args.input))
    sys.stderr.write(utils.color_str("DONE\n", font_color="green"))

    sys.stderr.write("Calu final result...")
    result_fname = "%s.submit" % (args.input)
    if args.output != None:
        result_fname = args.output

    calc_result(user_sim_dict, user2news, result_fname)
    sys.stderr.write(utils.color_str("DONE\n", font_color="green"))

