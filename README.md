# Shamir's Scheme
Simple implementation of Shamir's Secret Sharing Scheme over 256 ring. 
Due to small ring size (I guess) it provides very weak encryption for big t and n 
parameters (bigger then 10 already works not that good), so don't use it in 
real-life projects.

It works with binary files, encrypting each byte of file separately.