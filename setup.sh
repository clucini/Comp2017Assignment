gcc myfuse.c -lfuse -lm
rm fuse_test/a.out
cp a.out fuse_test/
cd fuse_test/
fusermount -u mount_point
echo complete
