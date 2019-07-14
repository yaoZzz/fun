import oo
import inspect
import funny

class Sub(oo.Object):
    def __init__(self):
        super(Sub, self).__init__()
        self.m1 = 1
        self.m2 = 2

        self.add_state_name("m1")
        self.add_state_name("m2")

sub = Sub()

fun = sub.create_object(b='b', a='a');
print fun
fun.debug()
#fun.add_state_name("hehehe")

fun = sub.create_object(b='b', a='ccc');

#fun.add_state_name("hehehe")
fun.debug()















