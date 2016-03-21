#! /usr/bin/env python

import sys
from optparse import OptionParser
import random
import math

def hfunc(index):
    if index == -1:
        return 'MISS'
    else:
        return 'HIT '

def vfunc(victim):
    if victim == -1:
        return '-'
    else:
        return str(victim)

#
# main program
#
parser = OptionParser()
parser.add_option('-a', '--addresses', default='-1',   help='a set of comma-separated pages to access; -1 means randomly generate',  action='store', type='string', dest='addresses')
parser.add_option('-i', '--init_memory', default='-1',   help='a set of comma-separated pages initial memory; -1 means []',  action='store', type='string', dest='init_memory')
parser.add_option('-p', '--policy', default='PFR',    help='replacement policy: FIFO, LRU, OPT, CLOCK, PFR',                action='store', type='string', dest='policy')
parser.add_option('-b', '--clockbits', default=1,      help='for CLOCK policy, how many clock bits to use',                          action='store', type='int', dest='clockbits')
parser.add_option('-f', '--pageframesize', default='3',    help='size of the physical page frame, in pages',                                  action='store', type='string', dest='pageframesize')
parser.add_option('-s', '--seed', default='0',         help='random number seed',                                                    action='store', type='string', dest='seed')
parser.add_option('-N', '--notrace', default=False,    help='do not print out a detailed trace',                                     action='store_true', dest='notrace')
parser.add_option('-c', '--compute', default=False,    help='compute answers for me',                                                action='store_true', dest='solve')
parser.add_option('-t', '--pf_threshold', default=3,    help='page fault_threshold',                                                action='store', type='int', dest='pf_threshold')

(options, args) = parser.parse_args()

print 'ARG addresses', options.addresses
print 'ARG init_memory', options.init_memory
print 'ARG policy', options.policy
print 'ARG clockbits', options.clockbits
print 'ARG pageframesize', options.pageframesize
print 'ARG seed', options.seed
print 'ARG notrace', options.notrace
print 'ARG compute', options.solve
print 'ARG pf_threshold', options.pf_threshold

addresses   = str(options.addresses)
init_memory = str(options.init_memory)
pageframesize   = int(options.pageframesize)
seed        = int(options.seed)
policy      = str(options.policy)
notrace     = options.notrace
clockbits   = int(options.clockbits)
pf_threshold = int(options.pf_threshold)

random.seed(seed)

addrList = []
if addresses == "-1":
    for i in range(10):
        addrList.append(random.randint(0, 3))
else:    
    addrList = [int(addr) for addr in addresses.split(',')]
print "addrList", addrList

if init_memory == "-1":
    init_memory = []
else:
    init_memory = [int(addr) for addr in init_memory.split(',')]
print "init_memory", init_memory

if options.solve == False:
    print 'Assuming a replacement policy of %s, and a physical page frame of size %d pages,' % (policy, pageframesize)
    print 'figure out whether each of the following page references hit or miss'

    for n in addrList:
        print 'Access: %d  Hit/Miss?  State of Memory?' % int(n)
    print ''

else:
    if notrace == False:
        print 'Solving...\n'

    # init memory structure
    # count = 0

    memory = init_memory
    
    hits = 0
    miss = 0

    if policy == 'FIFO':
        leftStr = 'FirstIn'
        riteStr = 'Lastin '
    elif policy == 'LRU':
        leftStr = 'LRU'
        riteStr = 'MRU'
    elif policy == 'OPT' or  policy == 'CLOCK':
        leftStr = 'Left '
        riteStr = 'Right'
    if policy == 'PFR':
        leftStr = 'Left '
        riteStr = 'Right'
        pass
    else:
        print 'Policy %s is not yet implemented' % policy
        exit(1)

    # track reference bits for clock
    ref   = {}
    last_pf = -1

    for addr in memory:
        ref[addr] = 0

    cdebug = False

    # need to generate addresses
    addrIndex = 0
    for clock_id, nStr in enumerate(addrList):
        # first, lookup
        n = int(nStr)

        try:
            idx = memory.index(n)
            hits = hits + 1
            if policy == 'LRU' :
                update = memory.remove(n)
                memory.append(n) # puts it on MRU side
            pf_span = -1
        except:
            # miss
            idx = -1
            miss = miss + 1
            pf_span = clock_id - last_pf
            last_pf = clock_id

        # idx = -1 (miss)  >=0 hit
        victim = -1  
        victims = []

        if idx == -1:
            # miss, replace?
            # print 'BUG count, pageframesize:', count, pageframesize
            if policy == "PFR":
                victims = []
                if pf_span > pf_threshold:
                    # put out some memory
                    recently_visited = set()
                    # actually we can build a list to save the most recently visited addr
                    for addr in addrList[clock_id - pf_threshold: clock_id]:
                        recently_visited.add(addr)
                    for i, x in enumerate(memory):
                        if x not in recently_visited:
                            del memory[i]
                            del ref[x]
                            victims.append(x)
                    if notrace == False:
                        # print "\t[%d,%d %d]" %(last_pf, clock_id, clock_id - last_pf) 
                        print "\t[Page Fault] span %d > threshold, delete %s" %(pf_span, victims)
                else:
                    if notrace == False:
                        print "\t[Page Fault] span %d <= threshold" %(pf_span)

            elif count == pageframesize and policy != "PFR":
                # must replace
                if policy == 'FIFO' or policy == 'LRU':
                    victim = memory.pop(0)
                elif policy == 'CLOCK':
                    if cdebug:
                        print 'REFERENCE TO PAGE', n
                        print 'MEMORY ', memory
                        print 'REF (b)', ref
                    # hack: for now, do random
                    # victim = memory.pop(int(random.random() * count))
                    victim = -1
                    while victim == -1:
                        page = memory[int(random.random() * count)]
                        if cdebug:
                            print '  scan page:', page, ref[page]
                        if ref[page] >= 1:
                            ref[page] -= 1
                        else:
                            # this is our victim
                            victim = page
                            memory.remove(page)
                            break

                    # remove old page's ref count
                    if page in memory:
                        assert('BROKEN')
                    del ref[victim]
                    if cdebug:
                        print 'VICTIM', page
                        print 'LEN', len(memory)
                        print 'MEM', memory
                        print 'REF (a)', ref

                elif policy == 'OPT':
                    maxReplace  = -1
                    replaceIdx  = -1
                    replacePage = -1
                    # print 'OPT: access %d, memory %s' % (n, memory) 
                    # print 'OPT: replace from FUTURE (%s)' % addrList[addrIndex+1:]
                    for pageIndex in range(0,count):
                        page = memory[pageIndex]
                        # now, have page 'page' at index 'pageIndex' in memory
                        whenReferenced = len(addrList)
                        # whenReferenced tells us when, in the future, this was referenced
                        for futureIdx in range(addrIndex+1,len(addrList)):
                            futurePage = int(addrList[futureIdx])
                            if page == futurePage:
                                whenReferenced = futureIdx
                                break
                        # print 'OPT: page %d is referenced at %d' % (page, whenReferenced)
                        if whenReferenced >= maxReplace:
                            # print 'OPT: ??? updating maxReplace (%d %d %d)' % (replaceIdx, replacePage, maxReplace)
                            replaceIdx  = pageIndex
                            replacePage = page
                            maxReplace  = whenReferenced
                            # print 'OPT: --> updating maxReplace (%d %d %d)' % (replaceIdx, replacePage, maxReplace)
                    victim = memory.pop(replaceIdx)
                    # print 'OPT: replacing page %d (idx:%d) because I saw it in future at %d' % (victim, replaceIdx, whenReferenced)
            else:
                # miss, but no replacement needed (page frame not full)
                victim = -1
                victioms = []
                count = count + 1

            # now add to memory
            memory.append(n)
            if cdebug:
                print 'LEN (a)', len(memory)

            if policy == 'PFR':
                for vc in victims:
                    assert(vc >= 0)
                    assert(vc not in memory)
            else:
                if victim != -1:
                    assert(victim not in memory)

        # after miss processing, update reference bit
        if n not in ref:
            ref[n] = 1
        else:
            ref[n] += 1
            if ref[n] > clockbits:
                ref[n] = clockbits
        
        if cdebug:
            print 'REF (a)', ref

        if notrace == False:
            print 'Access: %d  %s %s -> %12s <- %s Replaced:%s [Hits:%d Misses:%d]' % (n, hfunc(idx), leftStr, memory, riteStr, vfunc(victim), hits, miss)
        addrIndex = addrIndex + 1
        
    print ''
    print 'FINALSTATS hits %d   misses %d   hitrate %.2f' % (hits, miss, (100.0*float(hits))/(float(hits)+float(miss)))
    print ''

    
    
    







