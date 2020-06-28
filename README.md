# Software-Watchdog
Team Anant

A group of undergraduate students from BITS-Pilani, India are aiming at building a nanosatellite that will carry a hyperspectral imager as payload. This mission will serve as a proof of concept to highlight the feasibility of data-intensive imagery under power and computational constraints alongside other constraints on budget and expertise. The On-Board Computer (OBC) of the satellite is responsible for initiating data flow between onboard hardware and performing mission-critical computations like image compression.

This is a basic implementation of a software watchdog meant for a Nanosatellite. A software watchdog is basically a program that monitors the functioning of other processes in the queue. This code uses Real-time signals provided by Linux OS. Few dummy processes were created and the watchdog successfully killed the processor that took more than the stipulated time.
