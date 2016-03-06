# Succinct de Bruijn test

An educational implementation of Succinct de Bruijn as described in this paper:
http://alexbowe.com/succinct-debruijn-graphs/

Educational = just uses string and vector types instead of succinct bitvectors etc

Default make target runs the tests

Output
```
[section] simple debruijn test based on the example data in the paper
[section] test succinct against basic debruijn
[section] randomly generated tests for debruijn graph
[section] random number seed = 3965158422
[section] sequence = GCACTCCAGAAGTTAGGTGAA
[section] sequence = CAAAGTGGGAGACAACTGAAC
[section] sequence = CGGCGCTAAGTGCACCGGGTA
[section] sequence = TGCGAAAGGAGACAGATAATC
[section] sequence = TTTTGCGTGGCGATAGGTGCT
[section] rank select tests
```