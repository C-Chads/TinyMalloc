# Miniscule malloc and free implementation for freestanding environments

It also works hosted, obviously.

Porting should be pretty obvious. If you don't want a statically allocated buffer to be managed, you simply change the MANAGED_BUFFER_PTR
define, and remove the declaration.
