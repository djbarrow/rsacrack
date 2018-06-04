# rsacrack
These are my attempts to crack RSA.
RSA the encryption algorithm that secures all the Worlds Banks for since 1975 is based on a mathematical
problem which has stumped the best Mathematicians in the World since the Stone Age, the factoring of 
large numbers.

The guts of RSA is based on choosing two large Prime Numbers, numbers only divisible by one and itself,
typically over 100 decimal digits long. Multiplying them together, this is your magic number & then let 
some misfortunate hacker try to figure out what the 2 numbers are. If he can find them RSA is hacked. 
So simple a 10 year old can understand the problem, so why the hell cant a 12 year old crack it was beyond me.

On my visit to Prague in 2002 I was really confident that I was within hours of cracking it in several occasions 
& told my friend who was with me for the wild nights misbehaving that.

The insight I found out the hard way after probably one year spare time playing with the Chinese_remainder_theorem and
probabilities & reverse engineering the multiplications bit by bit in binary was that Primes are atoms,
and almost no knowledge of the factors can be ascertained by modulo tricks with the Chinese remainder theorem or by reverse engineering the multiplicatons. It´s really really hard.

