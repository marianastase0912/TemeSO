OBJ_LIST = so-cpp.obj hashmap.obj utilsFunctions.obj
CFLAGS   = /MD
EXE_NAME = so-cpp.exe
 
build : $(EXE_NAME)
 
$(EXE_NAME): $(OBJ_LIST)
        cl $(CFLAGS) /Fe$@ $**

so-cpp.obj: so-cpp.c
        cl $(CFLAGS) /c $**

hashmap.obj: hashmap.c hashmap.h
        cl $(CFLAGS) /c $**

utilsFunctions.obj: utilsFunctions.c utils.h
        cl $(CFLAGS) /c $**

clean:
        del *.obj *.exe
