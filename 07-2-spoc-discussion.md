# 同步互斥(lec 18) spoc 思考题


- 有"spoc"标记的题是要求拿清华学分的同学要在实体课上完成，并按时提交到学生对应的ucore_code和os_exercises的git repo上。

## 个人思考题

### 基本理解
 - 什么是信号量？它与软件同步方法的区别在什么地方？
 - 什么是自旋锁？它为什么无法按先来先服务方式使用资源？
 - 下面是一种P操作的实现伪码。它能按FIFO顺序进行信号量申请吗？
```
 while (s.count == 0) {  //没有可用资源时，进入挂起状态；
        调用进程进入等待队列s.queue;
        阻塞调用进程;
}
s.count--;              //有可用资源，占用该资源； 
```

> 参考回答： 它的问题是，不能按FIFO进行信号量申请。
> 它的一种出错的情况
```
一个线程A调用P原语时，由于线程B正在使用该信号量而进入阻塞状态；注意，这时value的值为0。
线程B放弃信号量的使用，线程A被唤醒而进入就绪状态，但没有立即进入运行状态；注意，这里value为1。
在线程A处于就绪状态时，处理机正在执行线程C的代码；线程C这时也正好调用P原语访问同一个信号量，并得到使用权。注意，这时value又变回0。
线程A进入运行状态后，重新检查value的值，条件不成立，又一次进入阻塞状态。
至此，线程C比线程A后调用P原语，但线程C比线程A先得到信号量。
```

### 信号量使用

 - 什么是条件同步？如何使用信号量来实现条件同步？
 - 什么是生产者-消费者问题？
 - 为什么在生产者-消费者问题中先申请互斥信息量会导致死锁？

### 管程

 - 管程的组成包括哪几部分？入口队列和条件变量等待队列的作用是什么？
 - 为什么用管程实现的生产者-消费者问题中，可以在进入管程后才判断缓冲区的状态？
 - 请描述管程条件变量的两种释放处理方式的区别是什么？条件判断中while和if是如何影响释放处理中的顺序的？

### 哲学家就餐问题

 - 哲学家就餐问题的方案2和方案3的性能有什么区别？可以进一步提高效率吗？

### 读者-写者问题

 - 在读者-写者问题的读者优先和写者优先在行为上有什么不同？
 - 在读者-写者问题的读者优先实现中优先于读者到达的写者在什么地方等待？
 
## 小组思考题

1. （spoc） 每人用python threading机制用信号量和条件变量两种手段分别实现[47个同步问题](07-2-spoc-pv-problems.md)中的一题。向勇老师的班级从前往后，陈渝老师的班级从后往前。请先理解[]python threading 机制的介绍和实例](https://github.com/chyyuu/ucore_lab/tree/master/related_info/lab7/semaphore_condition)
##一座小桥(最多只能承重两个人)横跨南北两岸，任意时刻同一方向只允许一人过桥，南侧桥段和北侧桥段较窄只能通过一人，桥中央一处宽敞，允许两个人通过或歇息。试用信号灯和PV操作写出南、北两岸过桥的同步算法。

桥上可能没有人，也可能有一人，也可能有两人。
·  两人同时过桥
·  两人都到中间
·  南(北)来者到北(南)段

共需要三个信号量，load用来控制桥上人数，初值为2，表示桥上最多有2人；north用来控制北段桥的使用，初值为1，用于对北段桥互斥；south用来控制南段桥的使用，初值为1，用于对南段桥互斥。
```
var load,north,south:semaphore;
load=2;
north=1;
south=1;
    GO_South()
      P(load);
      P(north);
    过北段桥;
    到桥中间;
      V(north);
      P(south);
    过南段桥;
    到达南岸;
      V(south);
      V(load);
    GO_North()
      P(load);
      P(south);
    过南段桥;
    到桥中间
      V(south);
      P(north);
    过北段桥;
    到达北岸
      V(north);
      V(load);
```
###信号量实现代码：（调了许久终于调出来了）
```c
#coding=utf-8
#!/usr/bin/env python

import threading  
import random  
import time  
class North(threading.Thread):  
    def __init__(self, threadName, semaphore0,semaphore1, semaphore2):  
        threading.Thread.__init__(self,name=threadName)  
        self.sleepTime=random.randrange(1,3)  
        self.road = semaphore0
        self.north = semaphore1
        self.south = semaphore2
    def run(self): 
        while True:
            self.road.acquire()
            self.north.acquire()
            print "pass north brdge"
            time.sleep(self.sleepTime) 
            print "pass middle brdge"
            self.north.release()
            self.south.acquire()
            print "pass north brdge"
            time.sleep(self.sleepTime) 
            print "pass middle brdge"
            self.south.release()
            self.road.release()

class South(threading.Thread):  
    def __init__(self, threadName, semaphore0,semaphore1, semaphore2):  
        threading.Thread.__init__(self,name=threadName)  
        self.sleepTime=random.randrange(1,2)  
        self.road = semaphore0
        self.north = semaphore1
        self.south = semaphore2

    def run(self):  
        while True:
            self.road.acquire()
            self.south.acquire()
            print "pass south brdge"
            time.sleep(self.sleepTime) 
            print "pass middle brdge"
            self.south.release()
            self.north.acquire()
            print "pass south brdge"
            time.sleep(self.sleepTime) 
            print "pass middle brdge"
            self.north.release()
            self.road.release()

threads=[]
road = threading.Semaphore(2)
north = threading.Semaphore(1)
south = threading.Semaphore(1)
threads.append(North("N-man", road, north,south))  
threads.append(North("N-man", road, north,south))  
threads.append(North("N-man", road, north,south))
threads.append(South("S-man", road, north,south))  
threads.append(South("S-man", road, north,south))  
threads.append(South("S-man", road, north,south))
for thread in threads: 
   thread.start() 
```