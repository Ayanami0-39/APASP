import subprocess

apasp_path = './build/ap_asp'
floyd_path = './build/floyd'
ggen_path = './build/ggen'

graph_str = subprocess.run([ggen_path], capture_output=True, text=True).stdout.strip()
floyd_out = subprocess.run([floyd_path], input=graph_str, capture_output=True, text=True).stdout.strip()
print("Floyd-Warshall:")
print(floyd_out)
apasp_out = subprocess.run([apasp_path], input=graph_str, capture_output=True, text=True).stdout.strip()
print("APASP:")
print(apasp_out)

apsp = floyd_out.split('\n')
apasp = apasp_out.split('\n')
flag = True
for r, (l1, l2) in enumerate(zip(apsp, apasp)):
    t1 = l1.split()
    t2 = l2.split()

    for c, (s1, s2) in enumerate(zip(t1, t2)):
        d1 = int(s1)
        d2 = int(s2)

        if d1 > d2 or d1+2 < d2:
            print("Error at row:{}, col:{}. {} - {}".format(r, c, d1, d2))
            flag = False
if flag:
    print("All data are within the allowable range of error.")