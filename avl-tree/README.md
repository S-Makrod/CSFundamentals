# AVL Tree Implementation

Consider an ADT consisting of a set S of distinct integers and the following operations:

**INSERT(S,x)**: Insert the element x into the set S. This operation has no effect if x is already in S.

**DELETE(S, x)**: Delete the element x from the set S. This operation has no effect if x is not in S.

**QUERY(S, x)**: Return true if x is in S and return false if x is not in S.

**CLOSEST-PAIR(S)**: Return two integers in S which are closest together in value.

In other words, if CLOSEST-PAIR(S) returns the integers a and b, then they must satisfy the condition. ∀𝑥∀𝑦(𝑥 ≠ 𝑦 → |𝑎 − 𝑏| ≤ |𝑥 − 𝑦|). It is an error if 𝑆 contains fewer than two elements. All the usual operations of INSERT, DELETE and QUERY must be performed in 𝑂(𝑙𝑜𝑔 𝑛) time, where 𝑛 = |𝑆| and CLOSEST-PAIR must be in 𝑂(1).
