
all: publish clean 

win: w-publish clean

w-publish:
	dotnet publish -r win-x64 -c Release -o .

publish:
	dotnet publish -r linux-x64 -c Release -o .

clean:
	rm -rf bin
	rm -rf obj
	rm -f ipk24chat-client.pdb

clear:
	rm -f ipk24chat-client.exe
	rm -f ipk24chat-client