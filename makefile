Target:=Delphes_Analysis
Mainfile:=Delphes_Analysis.cpp
#------------------------------------------------------------------------------
SrcSuf := cpp
IncSuf := h
ObjSuf := o
DepSuf := d
DllSuf := so

SrcDir := src
IncDir :=include
ObjDir := obj
DepDir := $(ObjDir)

INCDIRS = $(shell pwd)/include $(shell pwd)
EXTRAHDR = 
#------------------------------------------------------------------------------
# Architecture to compile for
ARCH          = linux
#ARCH          = solarisCC5

DELPHESPATH := /Users/teddy/workingspace/Delphes/Delphes-3.3.0

#MT2PATH := /Users/teddy/workingspace/Useful-Package/MT2 

ROOTCFLAGS   := $(shell root-config --cflags)
ROOTLIBS     := $(shell root-config --libs)
ROOTGLIBS    := $(shell root-config --glibs)
ROOTINCDIR := $(shell root-config --incdir)

DELPHESCFLAGS := -I$(DELPHESPATH) -I$(DELPHESPATH)/external
DELPHESLIBS  := -L$(DELPHESPATH) -lDelphes
DELPHESSPECIAL := -Xlinker -rpath $(DELPHESPATH)

#MT2CFLAGS := -I/home/thcollab/MC-Package/Support-Package/MT2/include/oxbridgekinetics-1.0
#MT2LIBS := -L/home/thcollab/MC-Package/Support-Package/MT2/lib/ -loxbridgekinetics-1.0
#MT2GLIBS := -L/home/thcollab/MC-Package/Support-Package/MT2/lib/ -loxbridgekinetics-1.0
#MT2SPECIAL := -Xlinker -rpath /home/thcollab/MC-Package/Support-Package/MT2/lib/


#INCDIRS	+= $(EXTRAHDR)
INCLUDES      = $(ROOTCFLAGS) $(MT2CFLAGS) $(addprefix -I, $(INCDIRS) ) 
CXX           = $(shell root-config --cxx)
LD            = $(shell root-config --ld)
SOFLAGS       = -shared

INCLUDESforROOTCINT = -I$(ROOTINCDIR) $(MT2CFLAGS) $(addprefix -I, $(INCDIRS) ) 

ifdef DEBUG
  CXXFLAGS    = -g -O0 
  LDFLAGS     = -g -O0
  DEFINES     =
else
  CXXFLAGS    = -O3
  LDFLAGS     = -O3
  DEFINES     = 
#  DEFINES     = -DNDEBUG
endif

DEFINES      += -DLINUXVERS -DHAS_SSTREAM
ifdef VERBOSE
DEFINES      += -DVERBOSE
endif
ifdef TESTCODE
DEFINES      += -DTESTCODE
endif

CXXFLAGS     += $(DEFINES) $(INCLUDES) $(DELPHESCFLAGS)
LIBS         += $(ROOTLIBS) $(SYSLIBS) $(DELPHESLIBS) $(MT2LIBS)
GLIBS        += $(ROOTGLIBS) $(SYSLIBS) $(MT2GLIBS)

#------------------------------------------------------------------------------
SRC = $(wildcard $(SrcDir)/*.$(SrcSuf))
HEAD = $(wildcard $(IncDir)/*.$(IncSuf))
OBJ           = $(patsubst $(SrcDir)/%.$(SrcSuf),$(ObjDir)/%.$(ObjSuf),$(SRC))
DEP           = $(patsubst $(SrcDir)/%.$(SrcSuf),$(DepDir)/%.$(DepSuf),$(SRC))
OBJS          = $(OBJ) $(ObjDir)/Event_VariablesDef.$(ObjSuf)
#$(ObjDir)/TreeReaderDef.$(ObjSuf)

#------------------------------------------------------------------------------
$(Target):	$(OBJS) $(Mainfile) $(SRC) $(HEAD)
		@echo "Compile $@..."
		@$(LD) $(LDFLAGS) $(CXXFLAGS) $(Mainfile) -o $@ -L./ $(OBJS) $(GLIBS) $(LIBS) $(MT2SPECIAL) $(DELPHESSPECIAL)
		@echo "--->$@ done"

$(ObjDir)/%.$(ObjSuf):$(SrcDir)/%.$(SrcSuf)
	@echo "Compile $<..."
	@$(CXX) $(CXXFLAGS) -o $@ -c $<


$(SrcDir)/Event_VariablesDef.$(SrcSuf):$(IncDir)/Event_Variables.h $(IncDir)/Event_VariablesLinkDef.h
	@echo "Generating dictionary $@..."
	@rootcint -f $@ -c $(INCLUDESforROOTCINT) $(DELPHESCFLAGS) $^

# $(SrcDir)/TreeReaderDef.$(SrcSuf):$(IncDir)/TreeReader.h $(IncDir)/TreeReaderLinkDef.h
# 	@echo "Generating dictionary $@..."
# 	rootcint -f $@ -c $(INCLUDESforROOTCINT) $(DELPHESCFLAGS) $^
#-------------------------------------------------------------------------------
clean:
	@rm -rf $(OBJS) $(Target) $(SrcDir)/Event_VariablesDef.$(SrcSuf) $(SrcDir)/Event_VariablesDef.$(IncSuf) $(SrcDir)/EachEventDef.$(SrcSuf) $(SrcDir)/EachEventDef.$(IncSuf) $(SrcDir)/TreeReaderDef.$(SrcSuf) $(SrcDir)/TreeReaderDef.$(IncSuf)
	@echo "--->Clean Done"
