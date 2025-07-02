# Energy-aware Markov chain device simulation

We provide a simple code that simulates a device with state (active/inactive) is defined by a Markov chain state machine.
The state machine is shown below:

<pre>
                 d1 
         +----------------+
         v                |
+----------+     d0     +--------+
| inactive | ---------->| active | 
+----------+            +--------+  
   |   ^                    |   ^
   \___| 1- d0              \___| 1- d1

</pre>


# Compilation

```bash
make clean
make
```

This will generate an executable called `energy`.
It has three parameters:

- d0: value for delta0
- p: probability that is define such as $p = \frac{\delta_0}{\delta_0 + \delta_1}$. This value will define an appropriate delta1.
- n: number of steps to run the code.

Running `energy` produces "markov_chain_results.csv"

# Analyzing the results

Run:

```
python3 view-csv.py
```

It creates a plot with the transitions, and computes delta0 and delta1 using only the data so you can check if the code is ok.