import os
import sys
import argparse
import subprocess
import time
import statistics
import re

parser = argparse.ArgumentParser(description='')
parser.add_argument('--no-fields', help='disable trying to interpret output as times', action='store_true')
parser.add_argument('--n', type=int, nargs=1, metavar='B', help='how many times should the program be run')
parser.add_argument('args', nargs=argparse.REMAINDER)
with_fields = True
n_runs = 1

args = dict(vars(parser.parse_args()))
if args['n'] and args['n'][0] > 1:
    n_runs = args['n'][0]

if args['no_fields']:
    with_fields = False


def stdout_to_values_dict(stdout):
    assert(type(stdout) == str)
    mdict = {}
    for ln in stdout.split('\n'):
        res = re.search(r'([^:]+):\s*(\d+(\.\d+)?)', ln) #name: 0.4343
        if res and res.group(1) and res.group(2):
            field_name  = res.group(1)
            value = float(res.group(2))
            mdict[field_name] = value
            # print(field_name, value)
        else:
            pass
            # print('%s doesnt match' % ln)
    return mdict
            
#ignore fields that are not there in all dicts
def drop_uncommon(list_of_dicts):
    if not list_of_dicts:
        return list_of_dicts
    fields = set(list_of_dicts[0].keys())
    rmv = set()
    for dikt in list_of_dicts:
        for field in fields:
            if field not in dikt:
                rmv.add(field)
    fields = fields.difference(rmv)
    # print(fields)
    field_aggr = {field: [] for field in fields}
    for dikt in list_of_dicts:
        for f in fields:
            # print(f,dikt)
            field_aggr[f].append(dikt[f])
    return field_aggr

def values_to_stats_dict(values):
    # print(values)
    assert(len(values) > 0)
    avgt, mint, maxt, medt, = statistics.mean(values), min(values), max(values), statistics.median(values)
    return {'avg': avgt, 'min' : mint, 'max' : maxt, 'median': medt}
def fmt_stats_dict(stats_dict):
    return ('avg: {}\n' +
            'min: {}\n' +
            'max: {}\n' +
            'median: {}\n').format(stats_dict['avg'], stats_dict['min'], stats_dict['max'], stats_dict['median'], )
def indent(string, ind):
    return '\n'.join(map(lambda x: (' '*ind) + x, string.split('\n')))

PROGS = []

class prog_run:
    def __init__(self, cmd_list):
        self.cmd_list = cmd_list
        self.stdout = ''
    def cmd(self):
        return ' '.join(self.cmd_list)
    def run(self, n):
        global with_fields
        cmd = self.cmd_list
        times = []
        stdout_values_dicts = []
        for i in range(n):
            beg = time.perf_counter()
            p = subprocess.run(args=cmd, stdout=subprocess.PIPE)
            if with_fields:
                stdout = p.stdout.decode('utf-8')
                stdout_values_dicts.append(stdout_to_values_dict(stdout))

            end = time.perf_counter()
            times.append(end-beg)
        stdout_values_dicts = drop_uncommon(stdout_values_dicts)
        time_stats_dict = values_to_stats_dict(times)
        out = ('' +
        'ran {} times\n' +
        'runtime:\n'
        '{}\n').format(n, indent(fmt_stats_dict(time_stats_dict), 4))
        if with_fields and len(stdout_values_dicts) > 0 :
            fields_stats_dicts = {}
            for field, vs in stdout_values_dicts.items() :
                if len(vs) > 0:
                    fields_stats_dicts[field] = values_to_stats_dict(vs)
            for field,ftimes in fields_stats_dicts.items():
                out = out + ('' +
                '"{}":\n' +
                '{}\n').format(field, indent(fmt_stats_dict(ftimes), 4))
        return out

def sep_into_progs(args):
    prog_cmds = []
    for cmd_arg in args:
        if cmd_arg == '...':
            PROGS.append(prog_run(prog_cmds))
            prog_cmds = list()
        else:
            sepd = cmd_arg.split(' ')
            prog_cmds.extend(sepd)
    if len(prog_cmds) > 0:
        PROGS.append(prog_run(prog_cmds))



sep_into_progs(args['args'])

for prog in PROGS:
    outp = prog.run(n_runs)
    print("cmd: '{}'\n{}".format(prog.cmd(), indent(outp, 2)))
