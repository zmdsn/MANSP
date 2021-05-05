#!/usr/bin/python
import networkx as nx
file_name = "gdb24.dat"
f = open(file_name,'r')
read_data = 0;
G = nx.Graph()
n=0
if (f) :
	for eachline in f:
		fstr = eachline
		containe_str =  fstr.split( )
		print( containe_str)
		if (containe_str[0]=="VERTICES") :
			n = int(containe_str[2])
		if (containe_str[0]=="DEPOSITO") :
			depot = int(containe_str[2]);
			break;
		if (containe_str[0]=="LISTA_ARISTAS_REQ") :
			read_data = 1;
			continue
		if (containe_str[0]=="END") :
			read_data = 0;
		if (read_data == 1) :
			containe_str1 =  containe_str[1].split(',')
			containe_str2 =  containe_str[2].split(')')
			print(containe_str1)
			print(containe_str2)
			print( [(int(containe_str1[0]),int(containe_str2[0]),int(containe_str[4]))] )
			G.add_weighted_edges_from([(int(containe_str1[0]),int(containe_str2[0]),int(containe_str[4]))])

#	print containe_str[0] 

f.close();
print (G.edges(),n)
G.add_weighted_edges_from([(0,1,float('inf'))])
#
#for i in range(0,np.size(col)+1):
#    G.add_node(i)
#
#for i in range(np.size(row)):
#    G.add_weighted_edges_from([(row[i],col[i],value[i])])
#
#
#path=nx.dijkstra_path(G, source=0, target=7)
print (nx.dijkstra_path_length(G, source=1, target=2))

file_out_name = file_name.replace(".d",".m")
print (file_out_name)
fo = open(file_out_name, "w")
for i in range(0,n+1) :
	for j in range(0,n+1) :
		len = nx.dijkstra_path_length(G,source=i,target=j)
		fo.write(str( len )+" ")
	fo.write("\n")
fo.close()

