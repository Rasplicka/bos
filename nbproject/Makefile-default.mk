#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/bos.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/bos.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

ifdef SUB_IMAGE_ADDRESS

else
SUB_IMAGE_ADDRESS_COMMAND=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=asm_macro.S app1.c app2.c app3.c spi.c rtc.S timer1.S pwm.c i2c.c portWriter.c test.c disp9341.c disp1306.c adc_scan.c usb_device_mm.c ubtn2.S touchXpt2046.c main.c globals.c fn.S graphics_hl.c graphics.c disp16_asm.S cfg_bits.c fnc.c periph.c user_apps.c coreFn.S

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/asm_macro.o ${OBJECTDIR}/app1.o ${OBJECTDIR}/app2.o ${OBJECTDIR}/app3.o ${OBJECTDIR}/spi.o ${OBJECTDIR}/rtc.o ${OBJECTDIR}/timer1.o ${OBJECTDIR}/pwm.o ${OBJECTDIR}/i2c.o ${OBJECTDIR}/portWriter.o ${OBJECTDIR}/test.o ${OBJECTDIR}/disp9341.o ${OBJECTDIR}/disp1306.o ${OBJECTDIR}/adc_scan.o ${OBJECTDIR}/usb_device_mm.o ${OBJECTDIR}/ubtn2.o ${OBJECTDIR}/touchXpt2046.o ${OBJECTDIR}/main.o ${OBJECTDIR}/globals.o ${OBJECTDIR}/fn.o ${OBJECTDIR}/graphics_hl.o ${OBJECTDIR}/graphics.o ${OBJECTDIR}/disp16_asm.o ${OBJECTDIR}/cfg_bits.o ${OBJECTDIR}/fnc.o ${OBJECTDIR}/periph.o ${OBJECTDIR}/user_apps.o ${OBJECTDIR}/coreFn.o
POSSIBLE_DEPFILES=${OBJECTDIR}/asm_macro.o.d ${OBJECTDIR}/app1.o.d ${OBJECTDIR}/app2.o.d ${OBJECTDIR}/app3.o.d ${OBJECTDIR}/spi.o.d ${OBJECTDIR}/rtc.o.d ${OBJECTDIR}/timer1.o.d ${OBJECTDIR}/pwm.o.d ${OBJECTDIR}/i2c.o.d ${OBJECTDIR}/portWriter.o.d ${OBJECTDIR}/test.o.d ${OBJECTDIR}/disp9341.o.d ${OBJECTDIR}/disp1306.o.d ${OBJECTDIR}/adc_scan.o.d ${OBJECTDIR}/usb_device_mm.o.d ${OBJECTDIR}/ubtn2.o.d ${OBJECTDIR}/touchXpt2046.o.d ${OBJECTDIR}/main.o.d ${OBJECTDIR}/globals.o.d ${OBJECTDIR}/fn.o.d ${OBJECTDIR}/graphics_hl.o.d ${OBJECTDIR}/graphics.o.d ${OBJECTDIR}/disp16_asm.o.d ${OBJECTDIR}/cfg_bits.o.d ${OBJECTDIR}/fnc.o.d ${OBJECTDIR}/periph.o.d ${OBJECTDIR}/user_apps.o.d ${OBJECTDIR}/coreFn.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/asm_macro.o ${OBJECTDIR}/app1.o ${OBJECTDIR}/app2.o ${OBJECTDIR}/app3.o ${OBJECTDIR}/spi.o ${OBJECTDIR}/rtc.o ${OBJECTDIR}/timer1.o ${OBJECTDIR}/pwm.o ${OBJECTDIR}/i2c.o ${OBJECTDIR}/portWriter.o ${OBJECTDIR}/test.o ${OBJECTDIR}/disp9341.o ${OBJECTDIR}/disp1306.o ${OBJECTDIR}/adc_scan.o ${OBJECTDIR}/usb_device_mm.o ${OBJECTDIR}/ubtn2.o ${OBJECTDIR}/touchXpt2046.o ${OBJECTDIR}/main.o ${OBJECTDIR}/globals.o ${OBJECTDIR}/fn.o ${OBJECTDIR}/graphics_hl.o ${OBJECTDIR}/graphics.o ${OBJECTDIR}/disp16_asm.o ${OBJECTDIR}/cfg_bits.o ${OBJECTDIR}/fnc.o ${OBJECTDIR}/periph.o ${OBJECTDIR}/user_apps.o ${OBJECTDIR}/coreFn.o

# Source Files
SOURCEFILES=asm_macro.S app1.c app2.c app3.c spi.c rtc.S timer1.S pwm.c i2c.c portWriter.c test.c disp9341.c disp1306.c adc_scan.c usb_device_mm.c ubtn2.S touchXpt2046.c main.c globals.c fn.S graphics_hl.c graphics.c disp16_asm.S cfg_bits.c fnc.c periph.c user_apps.c coreFn.S


CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/bos.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=32MM0256GPM036
MP_LINKER_FILE_OPTION=
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/asm_macro.o: asm_macro.S  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/asm_macro.o.d 
	@${RM} ${OBJECTDIR}/asm_macro.o 
	@${RM} ${OBJECTDIR}/asm_macro.o.ok ${OBJECTDIR}/asm_macro.o.err 
	@${FIXDEPS} "${OBJECTDIR}/asm_macro.o.d" "${OBJECTDIR}/asm_macro.o.asm.d" -t $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC} $(MP_EXTRA_AS_PRE)  -D__DEBUG -DICD3PlatformTool=1 -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/asm_macro.o.d"  -o ${OBJECTDIR}/asm_macro.o asm_macro.S -relaxed-math -DXPRJ_default=$(CND_CONF)  -legacy-libc  -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),-MD="${OBJECTDIR}/asm_macro.o.asm.d",--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--gdwarf-2,--defsym=__DEBUG=1,--defsym=ICD3PlatformTool=1
	
${OBJECTDIR}/rtc.o: rtc.S  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/rtc.o.d 
	@${RM} ${OBJECTDIR}/rtc.o 
	@${RM} ${OBJECTDIR}/rtc.o.ok ${OBJECTDIR}/rtc.o.err 
	@${FIXDEPS} "${OBJECTDIR}/rtc.o.d" "${OBJECTDIR}/rtc.o.asm.d" -t $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC} $(MP_EXTRA_AS_PRE)  -D__DEBUG -DICD3PlatformTool=1 -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/rtc.o.d"  -o ${OBJECTDIR}/rtc.o rtc.S -relaxed-math -DXPRJ_default=$(CND_CONF)  -legacy-libc  -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),-MD="${OBJECTDIR}/rtc.o.asm.d",--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--gdwarf-2,--defsym=__DEBUG=1,--defsym=ICD3PlatformTool=1
	
${OBJECTDIR}/timer1.o: timer1.S  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/timer1.o.d 
	@${RM} ${OBJECTDIR}/timer1.o 
	@${RM} ${OBJECTDIR}/timer1.o.ok ${OBJECTDIR}/timer1.o.err 
	@${FIXDEPS} "${OBJECTDIR}/timer1.o.d" "${OBJECTDIR}/timer1.o.asm.d" -t $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC} $(MP_EXTRA_AS_PRE)  -D__DEBUG -DICD3PlatformTool=1 -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/timer1.o.d"  -o ${OBJECTDIR}/timer1.o timer1.S -relaxed-math -DXPRJ_default=$(CND_CONF)  -legacy-libc  -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),-MD="${OBJECTDIR}/timer1.o.asm.d",--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--gdwarf-2,--defsym=__DEBUG=1,--defsym=ICD3PlatformTool=1
	
${OBJECTDIR}/ubtn2.o: ubtn2.S  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/ubtn2.o.d 
	@${RM} ${OBJECTDIR}/ubtn2.o 
	@${RM} ${OBJECTDIR}/ubtn2.o.ok ${OBJECTDIR}/ubtn2.o.err 
	@${FIXDEPS} "${OBJECTDIR}/ubtn2.o.d" "${OBJECTDIR}/ubtn2.o.asm.d" -t $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC} $(MP_EXTRA_AS_PRE)  -D__DEBUG -DICD3PlatformTool=1 -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/ubtn2.o.d"  -o ${OBJECTDIR}/ubtn2.o ubtn2.S -relaxed-math -DXPRJ_default=$(CND_CONF)  -legacy-libc  -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),-MD="${OBJECTDIR}/ubtn2.o.asm.d",--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--gdwarf-2,--defsym=__DEBUG=1,--defsym=ICD3PlatformTool=1
	
${OBJECTDIR}/fn.o: fn.S  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/fn.o.d 
	@${RM} ${OBJECTDIR}/fn.o 
	@${RM} ${OBJECTDIR}/fn.o.ok ${OBJECTDIR}/fn.o.err 
	@${FIXDEPS} "${OBJECTDIR}/fn.o.d" "${OBJECTDIR}/fn.o.asm.d" -t $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC} $(MP_EXTRA_AS_PRE)  -D__DEBUG -DICD3PlatformTool=1 -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/fn.o.d"  -o ${OBJECTDIR}/fn.o fn.S -relaxed-math -DXPRJ_default=$(CND_CONF)  -legacy-libc  -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),-MD="${OBJECTDIR}/fn.o.asm.d",--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--gdwarf-2,--defsym=__DEBUG=1,--defsym=ICD3PlatformTool=1
	
${OBJECTDIR}/disp16_asm.o: disp16_asm.S  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/disp16_asm.o.d 
	@${RM} ${OBJECTDIR}/disp16_asm.o 
	@${RM} ${OBJECTDIR}/disp16_asm.o.ok ${OBJECTDIR}/disp16_asm.o.err 
	@${FIXDEPS} "${OBJECTDIR}/disp16_asm.o.d" "${OBJECTDIR}/disp16_asm.o.asm.d" -t $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC} $(MP_EXTRA_AS_PRE)  -D__DEBUG -DICD3PlatformTool=1 -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/disp16_asm.o.d"  -o ${OBJECTDIR}/disp16_asm.o disp16_asm.S -relaxed-math -DXPRJ_default=$(CND_CONF)  -legacy-libc  -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),-MD="${OBJECTDIR}/disp16_asm.o.asm.d",--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--gdwarf-2,--defsym=__DEBUG=1,--defsym=ICD3PlatformTool=1
	
${OBJECTDIR}/coreFn.o: coreFn.S  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/coreFn.o.d 
	@${RM} ${OBJECTDIR}/coreFn.o 
	@${RM} ${OBJECTDIR}/coreFn.o.ok ${OBJECTDIR}/coreFn.o.err 
	@${FIXDEPS} "${OBJECTDIR}/coreFn.o.d" "${OBJECTDIR}/coreFn.o.asm.d" -t $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC} $(MP_EXTRA_AS_PRE)  -D__DEBUG -DICD3PlatformTool=1 -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/coreFn.o.d"  -o ${OBJECTDIR}/coreFn.o coreFn.S -relaxed-math -DXPRJ_default=$(CND_CONF)  -legacy-libc  -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),-MD="${OBJECTDIR}/coreFn.o.asm.d",--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--gdwarf-2,--defsym=__DEBUG=1,--defsym=ICD3PlatformTool=1
	
else
${OBJECTDIR}/asm_macro.o: asm_macro.S  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/asm_macro.o.d 
	@${RM} ${OBJECTDIR}/asm_macro.o 
	@${RM} ${OBJECTDIR}/asm_macro.o.ok ${OBJECTDIR}/asm_macro.o.err 
	@${FIXDEPS} "${OBJECTDIR}/asm_macro.o.d" "${OBJECTDIR}/asm_macro.o.asm.d" -t $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC} $(MP_EXTRA_AS_PRE)  -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/asm_macro.o.d"  -o ${OBJECTDIR}/asm_macro.o asm_macro.S -relaxed-math -DXPRJ_default=$(CND_CONF)  -legacy-libc  -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),-MD="${OBJECTDIR}/asm_macro.o.asm.d",--gdwarf-2
	
${OBJECTDIR}/rtc.o: rtc.S  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/rtc.o.d 
	@${RM} ${OBJECTDIR}/rtc.o 
	@${RM} ${OBJECTDIR}/rtc.o.ok ${OBJECTDIR}/rtc.o.err 
	@${FIXDEPS} "${OBJECTDIR}/rtc.o.d" "${OBJECTDIR}/rtc.o.asm.d" -t $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC} $(MP_EXTRA_AS_PRE)  -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/rtc.o.d"  -o ${OBJECTDIR}/rtc.o rtc.S -relaxed-math -DXPRJ_default=$(CND_CONF)  -legacy-libc  -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),-MD="${OBJECTDIR}/rtc.o.asm.d",--gdwarf-2
	
${OBJECTDIR}/timer1.o: timer1.S  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/timer1.o.d 
	@${RM} ${OBJECTDIR}/timer1.o 
	@${RM} ${OBJECTDIR}/timer1.o.ok ${OBJECTDIR}/timer1.o.err 
	@${FIXDEPS} "${OBJECTDIR}/timer1.o.d" "${OBJECTDIR}/timer1.o.asm.d" -t $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC} $(MP_EXTRA_AS_PRE)  -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/timer1.o.d"  -o ${OBJECTDIR}/timer1.o timer1.S -relaxed-math -DXPRJ_default=$(CND_CONF)  -legacy-libc  -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),-MD="${OBJECTDIR}/timer1.o.asm.d",--gdwarf-2
	
${OBJECTDIR}/ubtn2.o: ubtn2.S  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/ubtn2.o.d 
	@${RM} ${OBJECTDIR}/ubtn2.o 
	@${RM} ${OBJECTDIR}/ubtn2.o.ok ${OBJECTDIR}/ubtn2.o.err 
	@${FIXDEPS} "${OBJECTDIR}/ubtn2.o.d" "${OBJECTDIR}/ubtn2.o.asm.d" -t $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC} $(MP_EXTRA_AS_PRE)  -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/ubtn2.o.d"  -o ${OBJECTDIR}/ubtn2.o ubtn2.S -relaxed-math -DXPRJ_default=$(CND_CONF)  -legacy-libc  -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),-MD="${OBJECTDIR}/ubtn2.o.asm.d",--gdwarf-2
	
${OBJECTDIR}/fn.o: fn.S  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/fn.o.d 
	@${RM} ${OBJECTDIR}/fn.o 
	@${RM} ${OBJECTDIR}/fn.o.ok ${OBJECTDIR}/fn.o.err 
	@${FIXDEPS} "${OBJECTDIR}/fn.o.d" "${OBJECTDIR}/fn.o.asm.d" -t $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC} $(MP_EXTRA_AS_PRE)  -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/fn.o.d"  -o ${OBJECTDIR}/fn.o fn.S -relaxed-math -DXPRJ_default=$(CND_CONF)  -legacy-libc  -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),-MD="${OBJECTDIR}/fn.o.asm.d",--gdwarf-2
	
${OBJECTDIR}/disp16_asm.o: disp16_asm.S  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/disp16_asm.o.d 
	@${RM} ${OBJECTDIR}/disp16_asm.o 
	@${RM} ${OBJECTDIR}/disp16_asm.o.ok ${OBJECTDIR}/disp16_asm.o.err 
	@${FIXDEPS} "${OBJECTDIR}/disp16_asm.o.d" "${OBJECTDIR}/disp16_asm.o.asm.d" -t $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC} $(MP_EXTRA_AS_PRE)  -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/disp16_asm.o.d"  -o ${OBJECTDIR}/disp16_asm.o disp16_asm.S -relaxed-math -DXPRJ_default=$(CND_CONF)  -legacy-libc  -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),-MD="${OBJECTDIR}/disp16_asm.o.asm.d",--gdwarf-2
	
${OBJECTDIR}/coreFn.o: coreFn.S  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/coreFn.o.d 
	@${RM} ${OBJECTDIR}/coreFn.o 
	@${RM} ${OBJECTDIR}/coreFn.o.ok ${OBJECTDIR}/coreFn.o.err 
	@${FIXDEPS} "${OBJECTDIR}/coreFn.o.d" "${OBJECTDIR}/coreFn.o.asm.d" -t $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC} $(MP_EXTRA_AS_PRE)  -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/coreFn.o.d"  -o ${OBJECTDIR}/coreFn.o coreFn.S -relaxed-math -DXPRJ_default=$(CND_CONF)  -legacy-libc  -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),-MD="${OBJECTDIR}/coreFn.o.asm.d",--gdwarf-2
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/app1.o: app1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/app1.o.d 
	@${RM} ${OBJECTDIR}/app1.o 
	@${FIXDEPS} "${OBJECTDIR}/app1.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -DICD3PlatformTool=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -funroll-loops -fno-schedule-insns -mcci -MMD -MF "${OBJECTDIR}/app1.o.d" -o ${OBJECTDIR}/app1.o app1.c   -relaxed-math -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/app2.o: app2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/app2.o.d 
	@${RM} ${OBJECTDIR}/app2.o 
	@${FIXDEPS} "${OBJECTDIR}/app2.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -DICD3PlatformTool=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -funroll-loops -fno-schedule-insns -mcci -MMD -MF "${OBJECTDIR}/app2.o.d" -o ${OBJECTDIR}/app2.o app2.c   -relaxed-math -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/app3.o: app3.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/app3.o.d 
	@${RM} ${OBJECTDIR}/app3.o 
	@${FIXDEPS} "${OBJECTDIR}/app3.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -DICD3PlatformTool=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -funroll-loops -fno-schedule-insns -mcci -MMD -MF "${OBJECTDIR}/app3.o.d" -o ${OBJECTDIR}/app3.o app3.c   -relaxed-math -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/spi.o: spi.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/spi.o.d 
	@${RM} ${OBJECTDIR}/spi.o 
	@${FIXDEPS} "${OBJECTDIR}/spi.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -DICD3PlatformTool=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -funroll-loops -fno-schedule-insns -mcci -MMD -MF "${OBJECTDIR}/spi.o.d" -o ${OBJECTDIR}/spi.o spi.c   -relaxed-math -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/pwm.o: pwm.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/pwm.o.d 
	@${RM} ${OBJECTDIR}/pwm.o 
	@${FIXDEPS} "${OBJECTDIR}/pwm.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -DICD3PlatformTool=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -funroll-loops -fno-schedule-insns -mcci -MMD -MF "${OBJECTDIR}/pwm.o.d" -o ${OBJECTDIR}/pwm.o pwm.c   -relaxed-math -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/i2c.o: i2c.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/i2c.o.d 
	@${RM} ${OBJECTDIR}/i2c.o 
	@${FIXDEPS} "${OBJECTDIR}/i2c.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -DICD3PlatformTool=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -funroll-loops -fno-schedule-insns -mcci -MMD -MF "${OBJECTDIR}/i2c.o.d" -o ${OBJECTDIR}/i2c.o i2c.c   -relaxed-math -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/portWriter.o: portWriter.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/portWriter.o.d 
	@${RM} ${OBJECTDIR}/portWriter.o 
	@${FIXDEPS} "${OBJECTDIR}/portWriter.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -DICD3PlatformTool=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -funroll-loops -fno-schedule-insns -mcci -MMD -MF "${OBJECTDIR}/portWriter.o.d" -o ${OBJECTDIR}/portWriter.o portWriter.c   -relaxed-math -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/test.o: test.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/test.o.d 
	@${RM} ${OBJECTDIR}/test.o 
	@${FIXDEPS} "${OBJECTDIR}/test.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -DICD3PlatformTool=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -funroll-loops -fno-schedule-insns -mcci -MMD -MF "${OBJECTDIR}/test.o.d" -o ${OBJECTDIR}/test.o test.c   -relaxed-math -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/disp9341.o: disp9341.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/disp9341.o.d 
	@${RM} ${OBJECTDIR}/disp9341.o 
	@${FIXDEPS} "${OBJECTDIR}/disp9341.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -DICD3PlatformTool=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -funroll-loops -fno-schedule-insns -mcci -MMD -MF "${OBJECTDIR}/disp9341.o.d" -o ${OBJECTDIR}/disp9341.o disp9341.c   -relaxed-math -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/disp1306.o: disp1306.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/disp1306.o.d 
	@${RM} ${OBJECTDIR}/disp1306.o 
	@${FIXDEPS} "${OBJECTDIR}/disp1306.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -DICD3PlatformTool=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -funroll-loops -fno-schedule-insns -mcci -MMD -MF "${OBJECTDIR}/disp1306.o.d" -o ${OBJECTDIR}/disp1306.o disp1306.c   -relaxed-math -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/adc_scan.o: adc_scan.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/adc_scan.o.d 
	@${RM} ${OBJECTDIR}/adc_scan.o 
	@${FIXDEPS} "${OBJECTDIR}/adc_scan.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -DICD3PlatformTool=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -funroll-loops -fno-schedule-insns -mcci -MMD -MF "${OBJECTDIR}/adc_scan.o.d" -o ${OBJECTDIR}/adc_scan.o adc_scan.c   -relaxed-math -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/usb_device_mm.o: usb_device_mm.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/usb_device_mm.o.d 
	@${RM} ${OBJECTDIR}/usb_device_mm.o 
	@${FIXDEPS} "${OBJECTDIR}/usb_device_mm.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -DICD3PlatformTool=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -funroll-loops -fno-schedule-insns -mcci -MMD -MF "${OBJECTDIR}/usb_device_mm.o.d" -o ${OBJECTDIR}/usb_device_mm.o usb_device_mm.c   -relaxed-math -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/touchXpt2046.o: touchXpt2046.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/touchXpt2046.o.d 
	@${RM} ${OBJECTDIR}/touchXpt2046.o 
	@${FIXDEPS} "${OBJECTDIR}/touchXpt2046.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -DICD3PlatformTool=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -funroll-loops -fno-schedule-insns -mcci -MMD -MF "${OBJECTDIR}/touchXpt2046.o.d" -o ${OBJECTDIR}/touchXpt2046.o touchXpt2046.c   -relaxed-math -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/main.o: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	@${FIXDEPS} "${OBJECTDIR}/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -DICD3PlatformTool=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -funroll-loops -fno-schedule-insns -mcci -MMD -MF "${OBJECTDIR}/main.o.d" -o ${OBJECTDIR}/main.o main.c   -relaxed-math -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/globals.o: globals.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/globals.o.d 
	@${RM} ${OBJECTDIR}/globals.o 
	@${FIXDEPS} "${OBJECTDIR}/globals.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -DICD3PlatformTool=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -funroll-loops -fno-schedule-insns -mcci -MMD -MF "${OBJECTDIR}/globals.o.d" -o ${OBJECTDIR}/globals.o globals.c   -relaxed-math -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/graphics_hl.o: graphics_hl.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/graphics_hl.o.d 
	@${RM} ${OBJECTDIR}/graphics_hl.o 
	@${FIXDEPS} "${OBJECTDIR}/graphics_hl.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -DICD3PlatformTool=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -funroll-loops -fno-schedule-insns -mcci -MMD -MF "${OBJECTDIR}/graphics_hl.o.d" -o ${OBJECTDIR}/graphics_hl.o graphics_hl.c   -relaxed-math -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/graphics.o: graphics.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/graphics.o.d 
	@${RM} ${OBJECTDIR}/graphics.o 
	@${FIXDEPS} "${OBJECTDIR}/graphics.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -DICD3PlatformTool=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -funroll-loops -fno-schedule-insns -mcci -MMD -MF "${OBJECTDIR}/graphics.o.d" -o ${OBJECTDIR}/graphics.o graphics.c   -relaxed-math -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/cfg_bits.o: cfg_bits.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/cfg_bits.o.d 
	@${RM} ${OBJECTDIR}/cfg_bits.o 
	@${FIXDEPS} "${OBJECTDIR}/cfg_bits.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -DICD3PlatformTool=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -funroll-loops -fno-schedule-insns -mcci -MMD -MF "${OBJECTDIR}/cfg_bits.o.d" -o ${OBJECTDIR}/cfg_bits.o cfg_bits.c   -relaxed-math -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/fnc.o: fnc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/fnc.o.d 
	@${RM} ${OBJECTDIR}/fnc.o 
	@${FIXDEPS} "${OBJECTDIR}/fnc.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -DICD3PlatformTool=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -funroll-loops -fno-schedule-insns -mcci -MMD -MF "${OBJECTDIR}/fnc.o.d" -o ${OBJECTDIR}/fnc.o fnc.c   -relaxed-math -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/periph.o: periph.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/periph.o.d 
	@${RM} ${OBJECTDIR}/periph.o 
	@${FIXDEPS} "${OBJECTDIR}/periph.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -DICD3PlatformTool=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -funroll-loops -fno-schedule-insns -mcci -MMD -MF "${OBJECTDIR}/periph.o.d" -o ${OBJECTDIR}/periph.o periph.c   -relaxed-math -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/user_apps.o: user_apps.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/user_apps.o.d 
	@${RM} ${OBJECTDIR}/user_apps.o 
	@${FIXDEPS} "${OBJECTDIR}/user_apps.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -DICD3PlatformTool=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -funroll-loops -fno-schedule-insns -mcci -MMD -MF "${OBJECTDIR}/user_apps.o.d" -o ${OBJECTDIR}/user_apps.o user_apps.c   -relaxed-math -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
else
${OBJECTDIR}/app1.o: app1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/app1.o.d 
	@${RM} ${OBJECTDIR}/app1.o 
	@${FIXDEPS} "${OBJECTDIR}/app1.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -funroll-loops -fno-schedule-insns -mcci -MMD -MF "${OBJECTDIR}/app1.o.d" -o ${OBJECTDIR}/app1.o app1.c   -relaxed-math -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/app2.o: app2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/app2.o.d 
	@${RM} ${OBJECTDIR}/app2.o 
	@${FIXDEPS} "${OBJECTDIR}/app2.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -funroll-loops -fno-schedule-insns -mcci -MMD -MF "${OBJECTDIR}/app2.o.d" -o ${OBJECTDIR}/app2.o app2.c   -relaxed-math -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/app3.o: app3.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/app3.o.d 
	@${RM} ${OBJECTDIR}/app3.o 
	@${FIXDEPS} "${OBJECTDIR}/app3.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -funroll-loops -fno-schedule-insns -mcci -MMD -MF "${OBJECTDIR}/app3.o.d" -o ${OBJECTDIR}/app3.o app3.c   -relaxed-math -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/spi.o: spi.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/spi.o.d 
	@${RM} ${OBJECTDIR}/spi.o 
	@${FIXDEPS} "${OBJECTDIR}/spi.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -funroll-loops -fno-schedule-insns -mcci -MMD -MF "${OBJECTDIR}/spi.o.d" -o ${OBJECTDIR}/spi.o spi.c   -relaxed-math -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/pwm.o: pwm.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/pwm.o.d 
	@${RM} ${OBJECTDIR}/pwm.o 
	@${FIXDEPS} "${OBJECTDIR}/pwm.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -funroll-loops -fno-schedule-insns -mcci -MMD -MF "${OBJECTDIR}/pwm.o.d" -o ${OBJECTDIR}/pwm.o pwm.c   -relaxed-math -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/i2c.o: i2c.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/i2c.o.d 
	@${RM} ${OBJECTDIR}/i2c.o 
	@${FIXDEPS} "${OBJECTDIR}/i2c.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -funroll-loops -fno-schedule-insns -mcci -MMD -MF "${OBJECTDIR}/i2c.o.d" -o ${OBJECTDIR}/i2c.o i2c.c   -relaxed-math -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/portWriter.o: portWriter.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/portWriter.o.d 
	@${RM} ${OBJECTDIR}/portWriter.o 
	@${FIXDEPS} "${OBJECTDIR}/portWriter.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -funroll-loops -fno-schedule-insns -mcci -MMD -MF "${OBJECTDIR}/portWriter.o.d" -o ${OBJECTDIR}/portWriter.o portWriter.c   -relaxed-math -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/test.o: test.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/test.o.d 
	@${RM} ${OBJECTDIR}/test.o 
	@${FIXDEPS} "${OBJECTDIR}/test.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -funroll-loops -fno-schedule-insns -mcci -MMD -MF "${OBJECTDIR}/test.o.d" -o ${OBJECTDIR}/test.o test.c   -relaxed-math -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/disp9341.o: disp9341.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/disp9341.o.d 
	@${RM} ${OBJECTDIR}/disp9341.o 
	@${FIXDEPS} "${OBJECTDIR}/disp9341.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -funroll-loops -fno-schedule-insns -mcci -MMD -MF "${OBJECTDIR}/disp9341.o.d" -o ${OBJECTDIR}/disp9341.o disp9341.c   -relaxed-math -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/disp1306.o: disp1306.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/disp1306.o.d 
	@${RM} ${OBJECTDIR}/disp1306.o 
	@${FIXDEPS} "${OBJECTDIR}/disp1306.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -funroll-loops -fno-schedule-insns -mcci -MMD -MF "${OBJECTDIR}/disp1306.o.d" -o ${OBJECTDIR}/disp1306.o disp1306.c   -relaxed-math -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/adc_scan.o: adc_scan.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/adc_scan.o.d 
	@${RM} ${OBJECTDIR}/adc_scan.o 
	@${FIXDEPS} "${OBJECTDIR}/adc_scan.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -funroll-loops -fno-schedule-insns -mcci -MMD -MF "${OBJECTDIR}/adc_scan.o.d" -o ${OBJECTDIR}/adc_scan.o adc_scan.c   -relaxed-math -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/usb_device_mm.o: usb_device_mm.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/usb_device_mm.o.d 
	@${RM} ${OBJECTDIR}/usb_device_mm.o 
	@${FIXDEPS} "${OBJECTDIR}/usb_device_mm.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -funroll-loops -fno-schedule-insns -mcci -MMD -MF "${OBJECTDIR}/usb_device_mm.o.d" -o ${OBJECTDIR}/usb_device_mm.o usb_device_mm.c   -relaxed-math -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/touchXpt2046.o: touchXpt2046.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/touchXpt2046.o.d 
	@${RM} ${OBJECTDIR}/touchXpt2046.o 
	@${FIXDEPS} "${OBJECTDIR}/touchXpt2046.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -funroll-loops -fno-schedule-insns -mcci -MMD -MF "${OBJECTDIR}/touchXpt2046.o.d" -o ${OBJECTDIR}/touchXpt2046.o touchXpt2046.c   -relaxed-math -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/main.o: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	@${FIXDEPS} "${OBJECTDIR}/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -funroll-loops -fno-schedule-insns -mcci -MMD -MF "${OBJECTDIR}/main.o.d" -o ${OBJECTDIR}/main.o main.c   -relaxed-math -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/globals.o: globals.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/globals.o.d 
	@${RM} ${OBJECTDIR}/globals.o 
	@${FIXDEPS} "${OBJECTDIR}/globals.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -funroll-loops -fno-schedule-insns -mcci -MMD -MF "${OBJECTDIR}/globals.o.d" -o ${OBJECTDIR}/globals.o globals.c   -relaxed-math -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/graphics_hl.o: graphics_hl.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/graphics_hl.o.d 
	@${RM} ${OBJECTDIR}/graphics_hl.o 
	@${FIXDEPS} "${OBJECTDIR}/graphics_hl.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -funroll-loops -fno-schedule-insns -mcci -MMD -MF "${OBJECTDIR}/graphics_hl.o.d" -o ${OBJECTDIR}/graphics_hl.o graphics_hl.c   -relaxed-math -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/graphics.o: graphics.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/graphics.o.d 
	@${RM} ${OBJECTDIR}/graphics.o 
	@${FIXDEPS} "${OBJECTDIR}/graphics.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -funroll-loops -fno-schedule-insns -mcci -MMD -MF "${OBJECTDIR}/graphics.o.d" -o ${OBJECTDIR}/graphics.o graphics.c   -relaxed-math -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/cfg_bits.o: cfg_bits.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/cfg_bits.o.d 
	@${RM} ${OBJECTDIR}/cfg_bits.o 
	@${FIXDEPS} "${OBJECTDIR}/cfg_bits.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -funroll-loops -fno-schedule-insns -mcci -MMD -MF "${OBJECTDIR}/cfg_bits.o.d" -o ${OBJECTDIR}/cfg_bits.o cfg_bits.c   -relaxed-math -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/fnc.o: fnc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/fnc.o.d 
	@${RM} ${OBJECTDIR}/fnc.o 
	@${FIXDEPS} "${OBJECTDIR}/fnc.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -funroll-loops -fno-schedule-insns -mcci -MMD -MF "${OBJECTDIR}/fnc.o.d" -o ${OBJECTDIR}/fnc.o fnc.c   -relaxed-math -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/periph.o: periph.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/periph.o.d 
	@${RM} ${OBJECTDIR}/periph.o 
	@${FIXDEPS} "${OBJECTDIR}/periph.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -funroll-loops -fno-schedule-insns -mcci -MMD -MF "${OBJECTDIR}/periph.o.d" -o ${OBJECTDIR}/periph.o periph.c   -relaxed-math -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/user_apps.o: user_apps.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/user_apps.o.d 
	@${RM} ${OBJECTDIR}/user_apps.o 
	@${FIXDEPS} "${OBJECTDIR}/user_apps.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -funroll-loops -fno-schedule-insns -mcci -MMD -MF "${OBJECTDIR}/user_apps.o.d" -o ${OBJECTDIR}/user_apps.o user_apps.c   -relaxed-math -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/bos.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -g -mdebugger -DICD3PlatformTool=1 -mprocessor=$(MP_PROCESSOR_OPTION) -O3 -o dist/${CND_CONF}/${IMAGE_TYPE}/bos.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}         -relaxed-math -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)   -mreserve=data@0x0:0x1FC -mreserve=boot@0x1FC00490:0x1FC016FF -mreserve=boot@0x1FC00490:0x1FC00BEF  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D=__DEBUG_D,--defsym=ICD3PlatformTool=1,--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/bos.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION) -O3 -o dist/${CND_CONF}/${IMAGE_TYPE}/bos.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}         -relaxed-math -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml
	${MP_CC_DIR}\\xc32-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/bos.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} 
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
