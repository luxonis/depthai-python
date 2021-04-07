Device
======


Queues
######

When you define an output queue, the device can write to it at any point in time, and the host can read from it at any point in time.
So, there might be a cases when the host is reading very fast from the queue, and the queue, no matter its size, will stay empty most of
the time. But as we add things on the host side (additional processing, analysis etc), it may happen that the device will be writing to
the queue faster than the host can read from it. And then the packets in the queue will start to add up - and both maxSize and blocking
flags determine the behavior of the queue in this case. By default, the queue is blocking and its size is 30, so the device will put
30 packets at most, and when the limit is reached, it will hang on queue put call and wait until it can successfully complete this
call (so, waits for the host to consume the packet before putting a new one). Making the queue non-blocking will change its behavior
in this situation - instead of waiting, it will discard the oldest packet and add the new one, and then continue it's processing
loop (so it won't stop working, as it would happen with the blocking one). maxSize determines the size of the queue and also
helps to controll the memory usage - if the packet have 5MB of data, and the queue size is 30, this queue effectively stores
150MB of data in memory (and the packets can be really big, like when reading 4K RGB frames).
Also, decreasing the queue size to 1 will effectively mean "I want only the latest packet from the queue"

EDIT:
The 'stop working' - To not confuse with actually something not working.
What actually happens when blocking behavior is set is that the producer will wait (block) before sending more
messages - once the queue has space again, messages will continue coming.