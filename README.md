Consider a highway described as a sequence of service stations. Each service station is located at a distance from the beginning of the highway expressed in kilometers by a positive or zero integer. There are no two service stations having the same distance: each service station is therefore uniquely identified by its distance from the beginning of the highway.

Each service station is equipped with a fleet of electric rental vehicles. Each vehicle is distinguished by the range given by a battery charge, expressed in kilometers, by a positive integer. The fleet of vehicles of a single station includes at most 512 vehicles. By renting a car from an $s$ station, it is possible to reach all those whose distance from $s$ is less than or equal to the car's range.

A trip is identified by a sequence of service stations at which the driver stops. It therefore begins
at one service station and ends at another, passing through zero or more intermediate stations. Assume that the
driver cannot turn back during the trip, and rents a new car each time he stops at
a service station. Then, given two consecutive stops $s$ and $t$, $t$ must always be further from the start
than $s$, and $t$ must be reachable using one of the vehicles available at $s$.

The goal of the project is the following: given a pair of stations, plan the route with the least number of stages between them. In case there are multiple routes with the same minimum number of stops (i.e., tied), the route that prefers the stops with the shortest distance to the beginning of the highway should be chosen.

In other words, consider the set of $n$ equal routes $P =$ \{ $p_1, \space p_2, \space p_3, \space ...$ \} where each route is a tuple of $m$ elements $p_i = \langle p_{i, 1}, p_{i, 2}, ..., p_{i, m} \rangle$ that equates the distance from the highway start of each stop in order of travel.

The only $p_i$ route must be chosen such that there is no other $p_j$ route with the same $k$ final stages preceded by a stage with a shorter distance, i.e.: $\not\exists j, k : \langle p_{i, m-k+1}, ..., p_{i, m} \rangle = \langle p_{j, m-k+1}, ..., p_{j, m} \rangle \land p_{j, m-k} < p_{i, m-k}$

The following is an example of a highway. In this example, the correct path between the station at distance 20 and the station at distance 50 is 20 → 30 → 50 (and not 20 → 45 → 50). Note that, instead, 50 → 30 → 20 is the correct path between the station at distance 50 and the station at distance 20 (thus in the right-to-left direction).
