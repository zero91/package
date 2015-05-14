import sys
import os

from argparse import ArgumentParser

from jpyutils import utils

TRAIN_DATA_FNAME = "../../data_in/train_data.txt"

def read_train_data(train_data_fname):
    user2news = dict()
    news2user = dict()
    for line in open(train_data_fname, "r").xreadlines():
        line = line.strip()
        if line == '':
            continue

        try:
            user_id, news_id, view_time, title, content, post_time = line.split('\t')
            user2news.setdefault(user_id, set())
            news2user.setdefault(news_id, set())

            user2news[user_id].add(news_id.strip())
            news2user[news_id].add(user_id.strip())
        except Exception, e:
            sys.stderr.write("ERROR [%s], Line [%s]\n" % (e, line))
    return user2news, news2user

def calc_sim_user(user_data):
    """ Slow """
    user_sim = dict()
    user_cnt = 0
    for userid_1, news_set_1 in user_data.iteritems():
        user_sim.setdefault(userid_1, list())
        user_cnt += 1
        sys.stderr.write(utils.color_str("\r\tprocessing [%d] users" % (user_cnt)))

        for userid_2, news_set_2 in user_data.iteritems():
            if userid_1 == userid_2:
                continue
            user_sim[userid_1].append((userid_2, len(news_set_1 & news_set_2)))

    for userid in user_sim:
        user_sim[userid].sort(key=lambda item: item[1], reverse=True)
    return user_sim 

def calc_sim_user_2(user2news, news2user):
    """ Much faster than calc_sim_user """
    user_sim = dict()
    news_cnt = 0
    for news_id, user_set in news2user.iteritems():
        news_cnt += 1
        sys.stderr.write(utils.color_str("\r\tprocessing [%d] news" % (news_cnt)))
        for userid_1 in user_set:
            for userid_2 in user_set:
                if userid_1 >= userid_2:
                    continue
                user_sim.setdefault(userid_1, dict())
                user_sim.setdefault(userid_2, dict())

                user_sim[userid_1].setdefault(userid_2, 0)
                user_sim[userid_2].setdefault(userid_1, 0)
                user_sim[userid_1][userid_2] += 1
                user_sim[userid_2][userid_1] += 1
    
    for user_id, sim_user_dict in user_sim.iteritems():
        user_sim[user_id] = sorted(sim_user_dict.iteritems(), \
                                   key=lambda item: item[1], reverse=True)

    return user_sim

def parse_args():
    arg_parser = ArgumentParser()
    arg_parser.add_argument("-i", "--input", default=TRAIN_DATA_FNAME, help="Training File")
    arg_parser.add_argument("-o", "--output", required=True, help="The prefix of file which stores the result")
    return arg_parser.parse_args()

if __name__ == '__main__':
    args = parse_args()
    train_data_fname = args.input

    # step 1.
    sys.stderr.write(utils.color_str("Reading training data..."))
    user2news, news2user = read_train_data(train_data_fname)
    sys.stderr.write(utils.color_str("DONE\n"))

    tot_user_num = len(user2news)
    tot_news_num = len(news2user)
    sys.stderr.write("Total user num: %d\n" % (tot_user_num))
    sys.stderr.write("Total news num: %d\n" % (tot_news_num))

    # step 2.
    sys.stderr.write(utils.color_str("Calc sim user...\n"))
    #user_sim = calc_sim_user(user2news)
    user_sim = calc_sim_user_2(user2news, news2user)
    sys.stderr.write(utils.color_str("\nDONE\n"))

    # step 3.
    sys.stderr.write(utils.color_str("Saving result ..."))
    os.system("mkdir -p %s" % (os.path.dirname(os.path.realpath(args.output))))

    output_user2news = open("%s.user2news" % (args.output), "w")
    for userid, news_list in user2news.iteritems():
        output_user2news.write("%s\t%s\n" % (userid, ",".join(news_list)))
    output_user2news.close()

    output_file = open("%s.user_sim" % (args.output), "w")
    for userid, sim_list in user_sim.iteritems():
        output_file.write("%s\t%s\n" % (userid,
                                ",".join(["%s:%s" % (u, sim) for u, sim in sim_list if sim > 0])))
    output_file.close()
    sys.stderr.write(utils.color_str("DONE\n"))
