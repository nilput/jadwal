import sys
import subprocess
import time
import statistics
mx = 10
try:
    mx = int(sys.argv[1])
except:
    pass

cmd = sys.argv[2:]
times = []
for i in range(mx):
    beg = time.perf_counter()
    cmpl = subprocess.run(args=cmd)
    end = time.perf_counter()
    times.append(end-beg)
out = '''
ran {} times
avg: {}
min: {}
max: {}
median: {}
'''.format(mx, statistics.mean(times), min(times), max(times), statistics.median(times))
print(out)
