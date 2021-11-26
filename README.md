<div id="top"></div>

<br />
<div align="center">
  <h1 align="center">Computer Architecture Assignment 1</h1>
  <h3 align="center">Aristotle University of Thessaloniki</h3>
  <h4 align="center">School of Electrical & Computer Engineering</h4>
  <p align="center">
    Contributors: Kyriafinis Vasilis, Nikolaos Giannopoulos
    <br />
    Winter Semester 2021 - 2022
    <br />
    <br />
    <br />
    <br />
  </p>
</div>

# Simulation Parameters  (Question 1)

The file `starter_se.py` contains the script that defines the characteristics of the system to be emulated. The command to run the simulation is:

    ./build/ARM/gem5.opt configs/example/arm/starter_se.py --cpu="minor" "tests/test-progs/hello/bin/arm/linux/hello"

* The `configs/example/arm/starter_se.py` flag is the path to configuration script of the simulation
* The `--cpu="minor"` `"tests/test-progs/hello/bin/arm/linux/hello"` are the command line arguments for the configuration script
  * `--cpu="minor"`: Defines the type of the CPU to be used (default="atomic").
  * `"tests/test-progs/hello/bin/arm/linux/hello"`: The path to the executable binary that will run in the emulator.


In the lines 189 to 208 of the file `starter_se.py` the arguments are parsed. For this simulation only the above arguments were provided, so all the other arguments defaulted to the values specified in the script. 


* `--cpu-freq`: CPU clock frequency. Default = 1GHz
* `--num-cores`: Number of cores of the CPU. Default = 1
* `--mem-type`: Type of system memory. Default = `DDR3_1600_8x8` meaning Transfer rate = 1.6 x 8 x 8 x 1/8 = 12.8GBps
* `--mem-channels`: The number of memory channels. Default = 2
* `--mem-ranks`: Number of ranks per memory channel. Default = None
* `--mem-size`: Memory size. Default = 2GB


## The main modules of the system

* **_System Clock_**: The system clock has a default frequency of 1GHz and it is not the same clock with the CPU clock. To change the system clock frequency the value in the below line must be changed.
  
      self.clk_domain = SrcClockDomain(clock="1GHz",voltage_domain=self.voltage_domain)

* **_Memory bus_**: The memory bus connects the CPU with the system memory.
  
      self.membus = SystemXBar()

* **_CPU_**: The type and the frequency of the CPU are determined by the command line arguments. To change the default frequency of the CPU the `--cpu-freq` argument must be passed to the script.
  
      devices.`path/to/binary`(self, args.num_cores, args.cpu_freq, "1.2V", *cpu_types[args.cpu])


Depending on the types of the CPU it is determined if the system will have cache memories. If the memory model is not Atomic, which means the data are read from the memory with no delays, 2 cache levels are created. 

* **_L1 Cache_**: L1 cache is private to each core, so that every core can only access his L1 cache. 
  
      self.cpu_cluster.addL1()

* **_L2 Cache_**: L2 Cache is shared between all of the cores.
  
      self.cpu_cluster.addL2(self.cpu_cluster.clk_domain)

<br/>

# Run Statistics (Question 2)

The `stats.txt` file contains information about the simulation from all the SimObjects. At the end of the simulation the statistics are automatically dumped to the file. Some important information can be derived from the bellow values:

* **_sim_seconds_**: Number of seconds simulated (0.000035 s). This is the time the binary took to execute in the simulator.
* **_sim_insts_**: The total number of instructions that where simulated (5027 instructions).
* **_host_inst_rate_**: The instructions per second of the simulator on the host machine (118842 inst/s). Basically this is the performance of the gem5 simulator.


<br />

# CPI (Question 3)

In order to calculate the CPI (Clocks per instruction) the total cache misses for L1 data and instructions and the total cache misses for L2 are needed. Also the miss penalties and hit penalties for both levels of cache and finally the total number of instruction executed are required. The mathematical type is very simple: 

<br/>

<div align="center">
<a href="https://www.codecogs.com/eqnedit.php?latex=\dpi{100}&space;\bg_white&space;\fn_jvn&space;CPI&space;=&space;1&space;&plus;&space;\frac{(\text{IL1.miss\_num&space;&plus;&space;\text{DL1.miss\_num}})\times&space;6&space;&plus;&space;\text{L2.miss\_num}\times&space;50}{\text{Total\_Inst\_num}}" target="_blank"><img src="https://latex.codecogs.com/png.latex?\dpi{100}&space;\bg_white&space;\fn_jvn&space;CPI&space;=&space;1&space;&plus;&space;\frac{(\text{IL1.miss\_num&space;&plus;&space;\text{DL1.miss\_num}})\times&space;6&space;&plus;&space;\text{L2.miss\_num}\times&space;50}{\text{Total\_Inst\_num}}" title="CPI = 1 + \frac{(\text{IL1.miss\_num + \text{DL1.miss\_num}})\times 6 + \text{L2.miss\_num}\times 50}{\text{Total\_Inst\_num}}" /></a>
</div>
<br/>

The first 1 is there because if we miss on the L1 cache we have already paid for the hit penalty. Afterward all the cycles spent dealing with the miss are counted and finally the average per instruction is calculated by dividing with the number of the total instructions.

<br/>

In our case the number of misses on the L1 cache are:

* 327 misses for instruction cache:  
      
      system.cpu_cluster.cpus.icache.demand_misses::.cpu_cluster.cpus.inst  327  # number of demand (read+write) misses

* 177 misses for data cache:
    
      system.cpu_cluster.cpus.dcache.demand_misses::.cpu_cluster.cpus.data  177  # number of demand (read+write) misses

The L2 cache is unified, meaning there is no instruction and data part. So the misses for the L2 cache are calculated by adding the misses from L2 instructions and L2 data misses.

In our case the number of misses on the L2 cache are:

* 327 misses for instructions:
      
      system.cpu_cluster.l2.demand_misses::.cpu_cluster.cpus.inst  327   # number of demand (read+write) misses

* 147 misses for data:
      
      system.cpu_cluster.l2.demand_misses::.cpu_cluster.cpus.inst  327    # number of demand (read+write) misses

The number of instruction misses are the same on the L1 and L2 caches because the different instructions executed fitted inside the L1 instruction cache and the L2 cache. That means the instructions where requested by the CPU and were loaded from the DRAM so for every instruction fetched there was an initial compulsory miss.
<br/>

On the other hand the data misses are more for the L1 cache. This is the case because data where used more than once so after the first compulsory miss and the data fetch from the DRAM to the L2 cache the data were small enough to remain inside L2 cache but not small enough to fit in the L1 cache all at once. So there were cases that data was requested by the CPU the L1 cache got a miss bu the L2 cache got a hit.
<br/>

Finally the total instructions simulated were:

    sim_insts  5027    # Number of instructions simulated

Applying the above data to the equation gives `CPI = 6.32 cycles per instruction`

<br/>

# Sources

[A Tutorial on the Gem5 Memory Model](https://nitish2112.github.io/post/gem5-memory-model/)

[Creating SimObjects in the memory system](http://learning.gem5.org/book/part2/memoryobject.html)

[Understanding gem5 statistics and output](http://learning.gem5.org/book/part1/gem5_stats.html)

[Understanding gem5 statistics and output Gem5](https://www.gem5.org/documentation/learning_gem5/part1/gem5_stats/)

[What are system clock and CPU clock](https://cs.stackexchange.com/questions/32149/what-are-system-clock-and-cpu-clock-and-what-are-their-functions)