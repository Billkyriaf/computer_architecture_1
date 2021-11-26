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

# Simulation Parameters (Question 1)

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