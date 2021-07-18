# Miniscule malloc and free implementation for freestanding C

(It also works hosted, obviously)

Porting should be pretty obvious. If you don't want a statically allocated buffer to be managed, 
you simply change the MANAGED_BUFFER_PTR define, and remove the declaration of the static buffer in the .c file.

the mutex lock and unlock defines are for you to insert your own mutex guards for your platform.

the USE_ALIGN define is only for hosted implementations and the like. in a freestanding environment, you would
probably just use a literal integer for the pointer, to point to the beginning of the managed memory buffer.

This project was written in about 15 minutes with the aim of helping OSdev nerds who want minimal stuff
for their meme kernels.

This repository is fully public domain.
