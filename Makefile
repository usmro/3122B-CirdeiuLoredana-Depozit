all:
	g++ -Iinclude src/*.cpp -o nexus_hub

clean:
	rm -f nexus_hub