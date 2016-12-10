import math
import time
def big_circle(lon1, lat1, lon2, lat2):
    radius = 3965 
    x = math.pi / 180.0

    a = (90.0 - lat1) * (x)
    b = (90.0 - lat2) * (x)
    theta = (lon2 - lon1) * (x)
    c = math.acos(math.cos(a) * math.cos(b) +
        math.sin(a) * math.sin(b)*math.cos(theta))
    return c*radius
lon1, lat1, lon2, lat2 = -72.345, 34.323, -61.823, 54.826
st = time.time()
for i in range(5000000):
    result = big_circle(lon1, lat1, lon2, lat2)
print time.time() - st