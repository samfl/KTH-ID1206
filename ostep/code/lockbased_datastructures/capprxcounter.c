/* When using a concurrent simple counter, performance is a problem. */
/* We want to achieve "perfect" scaling: Being able to update a counter just as quickly on multiple threads as on a single thread. */
/* Updating a counter is extremely fast on a single thread */
/* Updating a counter using 2 threads is extremely slow */
/* Updating a counter using more than 2 threads just makes it even slower. */
/* For some workloads, we need SCALABLE counting! */

/* One of these approaches, are called Approximate Counter */
/* Local physical counters, one per CPU core, and a global counter is used to represent a single logical counter. */

/* Due to the local counter, a thread can update it without contention (it is synchronized with local lock) */
/* To keep the global counter up to date, the local values are periodically transferred to the global counter (synchornized with global lock) */
/* After transferring to the global, the local counter can be resetted. */

/* Implementation in Figure 29.5 - Approximate Counter Implementation */
