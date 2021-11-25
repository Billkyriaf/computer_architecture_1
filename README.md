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

# Παράμετροι Simulation

Στο αρχείο `starter_se.py` βρίσκεται το script στο οποίο δηλώθηκαν τα χαρακτηριστικά του συστήματος για την εξομοίωση. Η εντολή που έτρεξε για την εξομοίωση αυτή ήταν: 

    ./build/ARM/gem5.opt configs/example/arm/starter_se.py --cpu="minor" "tests/test-progs/hello/bin/arm/linux/hello"


* Το `configs/example/arm/starter_se.py` δηλώνει ποιό configuration script θα χρησιμοποιηθεί για την τρέχουσα εξομοίωση
* Τα `--cpu="minor"` `"tests/test-progs/hello/bin/arm/linux/hello"` είναι τα command line arguments για το configuration script
  * `--cpu="minor"`: Δηλώνει τον τύπο του cpu που θα χρησιμοποιηθεί (default="atomic").
  * `"tests/test-progs/hello/bin/arm/linux/hello"`: Το path στο εκτελέσιμο που θα τρέξει σε αυτή την εξομοίωση.

Στο αρχείο `starter_se.py` στις γραμμές 189 μέχρι 208 γίνονται parse τα arguments για το πρόγραμμα. Στην συγκεκριμένη εξομοίωση δόθηκαν μόνο τα δύο παραπάνω ορίσματα συνεπώς στα υπόλοιπα δόθηκαν οι default τιμές:

* `--cpu-freq`: Η συχνότητα του ρολογιού του επεξεργαστή. Default = 1GHz
* `--num-cores`: Το πλήθος των πυρήνων του επεξεργαστή. Default = 1
* `--mem-type`: Το είδος της μνήμης του συστήματος. Default = `DDR3_1600_8x8` δηλαδή Transfer rate = 1.6 x 8 x 8 x 1/8 = 12.8GBps
* `--mem-channels`: Το πλήθος των καναλιών της μνήμης. Default = 2
* `--mem-ranks`: Το πλήθος των ranks ανά κανάλι μνήμης. Default = None
* `--mem-size`: Το μέγεθος της μνήμης. Default = 2GB

Τα βασικά μέρη του συστήματος που δημιουργούνται στο configuration script είναι:

* **_Ρολόι συστήματος_**: Το ρολόι έχει default συχνότητα 1GHz και είναι διαφορετικό από αυτό του επεξεργαστή. Για να αλλάξει η συχνότητα αυτή πρέπει αν γίνει αλλαγή στην παρακάτω γραμμή
  
      self.clk_domain = SrcClockDomain(clock="1GHz",voltage_domain=self.voltage_domain)

* **_Memory bus_**: Το memory bus συνδέει τον επεξεργαστή με την DRAM.
  
      self.membus = SystemXBar()

* **_CPU_**: Ο τύπος και η συχνότητα του CPU καθορίζεται απο τα ορίσματα που δίνονται στο script. Για να αλλάξει η συχνότητα του επεξεργαστή πρέπει να δοθεί το argument `--cpu-freq`.
  
      devices.`path/to/binary`(self, args.num_cores, args.cpu_freq, "1.2V", *cpu_types[args.cpu])

Αναλόγως με τον τύπο του CPU καθορίζεται το αν θα υπάρχουν μνήμες Cache η όχι. Αν το μοντέλο της μνήμης δεν είναι atomic, δηλαδή τα δεδομένα διαβάζονται απο την μνήμη με πιο ρεαλιστικό τρόπο, δημιουργούνται 2 επίπεδα Cache. 

* **_L1 Cache_**: Η L1 cache είναι private για τον κάθε πυρήνα αυτό σημαίνει ότι ο κάθε πυρήνας έχει την δική του μνήμη και δεν έχει πρόσβαση σε αυτές των άλλων πυρήνων. 
  
      self.cpu_cluster.addL1()

* **_L2 Cache_**: Η L2 Cache είναι κοινή για όλους τους πυρήνες.
  
      self.cpu_cluster.addL2(self.cpu_cluster.clk_domain)