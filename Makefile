all:
	python3 gen.py && mv tmp/*.h tmp/src && mv tmp/*.cpp tmp/src
