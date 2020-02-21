# Objects
OBJS := \
./hidapi/libusb/hid.o \
./main.o \
./rift.o

# Dependencies
C_DEPS := $(OBJS:%.o=%.d)

# Libraries
LIBS := \
-lusb-1.0

# All target
all: riftDriverPi

# Tool invocations
riftDriverPi: $(OBJS)
	gcc -pthread -o "riftDriverPi" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo ' '

# Other Targets
clean:
	-rm -rf $(EXECUTABLES)$(OBJS)$(C_DEPS) riftDriver
	-@echo ' '

%.o: %.c
	gcc -I/usr/include/libusb-1.0 -O0 -g3 -Wall -c -fmessage-length=0 `pkg-config libusb-1.0 --cflags` -fPIC -pthread -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo ' '

.PHONY: all clean dependents
