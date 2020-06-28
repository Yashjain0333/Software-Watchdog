# Software-Watchdog
Team Anant

This is a basic implementation of a software watchdog meant for a Nanosatellite, built for the student satellite team of BITS Pilani (Team Anant).
A software watchdog is basically a program that monitors the functioning of other processes in the queue.
This code uses Real-time signals provided by Linux OS. Few dummy processes were created and the watchdog successfully killed the processor that took more than the stipulated time.
