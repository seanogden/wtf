import hyperclient
c = hyperclient.Client('127.0.0.1', 1981)
c.add_space('''
space wtf
key path 
attributes map(string, string) blockmap
subspace blockmap 
create 8 partitions
tolerate 2 failures
''')
