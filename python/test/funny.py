import time
for i in range(2):
    time.sleep(1)
    print 'import funny....', i

class funny(object):
    #def __init__(self, a, b, c = 1, *v, **k):
    def __init__(self, a, b):
        self.a = a
        self.b = b
        '''
        self.c = c
        self.v = v
        self.k = k
        '''

    def debug(self):
        print "funny debug:", self.a, self.b
