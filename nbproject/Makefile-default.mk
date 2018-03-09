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
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/BOS2.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/BOS2.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
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
SOURCEFILES_QUOTED_IF_SPACED=asm_macro.S clock_cfgBits_mm.c clock_cfgBits_mz.c main.c globals.c timer1.S core_fn.S rtc.S change_notif.S ubtn2.S periph.c periph_fn.S app1.c app2.c app3.c user_apps.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/asm_macro.o ${OBJECTDIR}/clock_cfgBits_mm.o ${OBJECTDIR}/clock_cfgBits_mz.o ${OBJECTDIR}/main.o ${OBJECTDIR}/globals.o ${OBJECTDIR}/timer1.o ${OBJECTDIR}/core_fn.o ${OBJECTDIR}/rtc.o ${OBJECTDIR}/change_notif.o ${OBJECTDIR}/ubtn2.o ${OBJECTDIR}/periph.o ${OBJECTDIR}/periph_fn.o ${OBJECTDIR}/app1.o ${OBJECTDIR}/app2.o ${OBJECTDIR}/app3.o ${OBJECTDIR}/user_apps.o
POSSIBLE_DEPFILES=${OBJECTDIR}/asm_macro.o.d ${OBJECTDIR}/clock_cfgBits_mm.o.d ${OBJECTDIR}/clock_cfgBits_mz.o.d ${OBJECTDIR}/main.o.d ${OBJECTDIR}/globals.o.d ${OBJECTDIR}/timer1.o.d ${OBJECTDIR}/core_fn.o.d ${OBJECTDIR}/rtc.o.d ${OBJECTDIR}/change_notif.o.d ${OBJECTDIR}/ubtn2.o.d ${OBJECTDIR}/periph.o.d ${OBJECTDIR}/periph_fn.o.d ${OBJECTDIR}/app1.o.d ${OBJECTDIR}/app2.o.d ${OBJECTDIR}/app3.o.d ${OBJECTDIR}/user_apps.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/asm_macro.o ${OBJECTDIR}/clock_cfgBits_mm.o ${OBJECTDIR}/clock_cfgBits_mz.o ${OBJECTDIR}/main.o ${OBJECTDIR}/globals.o ${OBJECTDIR}/timer1.o ${OBJECTDIR}/core_fn.o ${OBJECTDIR}/rtc.o ${OBJECTDIR}/change_notif.o ${OBJECTDIR}/ubtn2.o ${OBJECTDIR}/periph.o ${OBJECTDIR}/periph_fn.o ${OBJECTDIR}/app1.o ${OBJECTDIR}/app2.o ${OBJECTDIR}/app3.o ${OBJECTDIR}/user_apps.o

# Source Files
SOURCEFILES=asm_macro.S clock_cfgBits_mm.c clock_cfgBits_mz.c main.c globals.c timer1.S core_fn.S rtc.S change_notif.S ubtn2.S periph.c periph_fn.S app1.c app2.c app3.c user_apps.c


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
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/BOS2.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=32MZ2048ECM144
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
	@${FIXDEPS} "${OBJECTDIR}/asm_macro.o.d" "${OBJECTDIR}/asm_macro.o.asm.d" -t $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC} $(MP_EXTRA_AS_PRE)  -D__DEBUG -DPKOBSKDEPlatformTool=1 -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/asm_macro.o.d"  -o ${OBJECTDIR}/asm_macro.o asm_macro.S -relaxed-math -DXPRJ_default=$(CND_CONF)  -legacy-libc  -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),-MD="${OBJECTDIR}/asm_macro.o.asm.d",--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--gdwarf-2,--defsym=__DEBUG=1,--defsym=PKOBSKDEPlatformTool=1
	
${OBJECTDIR}/timer1.o: timer1.S  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/timer1.o.d 
	@${RM} ${OBJECTDIR}/timer1.o 
	@${RM} ${OBJECTDIR}/timer1.o.ok ${OBJECTDIR}/timer1.o.err 
	@${FIXDEPS} "${OBJECTDIR}/timer1.o.d" "${OBJECTDIR}/timer1.o.asm.d" -t $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC} $(MP_EXTRA_AS_PRE)  -D__DEBUG -DPKOBSKDEPlatformTool=1 -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/timer1.o.d"  -o ${OBJECTDIR}/timer1.o timer1.S -relaxed-math -DXPRJ_default=$(CND_CONF)  -legacy-libc  -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),-MD="${OBJECTDIR}/timer1.o.asm.d",--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--gdwarf-2,--defsym=__DEBUG=1,--defsym=PKOBSKDEPlatformTool=1
	
${OBJECTDIR}/core_fn.o: core_fn.S  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/core_fn.o.d 
	@${RM} ${OBJECTDIR}/core_fn.o 
	@${RM} ${OBJECTDIR}/core_fn.o.ok ${OBJECTDIR}/core_fn.o.err 
	@${FIXDEPS} "${OBJECTDIR}/core_fn.o.d" "${OBJECTDIR}/core_fn.o.asm.d" -t $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC} $(MP_EXTRA_AS_PRE)  -D__DEBUG -DPKOBSKDEPlatformTool=1 -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/core_fn.o.d"  -o ${OBJECTDIR}/core_fn.o core_fn.S -relaxed-math -DXPRJ_default=$(CND_CONF)  -legacy-libc  -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),-MD="${OBJECTDIR}/core_fn.o.asm.d",--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--gdwarf-2,--defsym=__DEBUG=1,--defsym=PKOBSKDEPlatformTool=1
	
${OBJECTDIR}/rtc.o: rtc.S  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/rtc.o.d 
	@${RM} ${OBJECTDIR}/rtc.o 
	@${RM} ${OBJECTDIR}/rtc.o.ok ${OBJECTDIR}/rtc.o.err 
	@${FIXDEPS} "${OBJECTDIR}/rtc.o.d" "${OBJECTDIR}/rtc.o.asm.d" -t $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC} $(MP_EXTRA_AS_PRE)  -D__DEBUG -DPKOBSKDEPlatformTool=1 -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/rtc.o.d"  -o ${OBJECTDIR}/rtc.o rtc.S -relaxed-math -DXPRJ_default=$(CND_CONF)  -legacy-libc  -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),-MD="${OBJECTDIR}/rtc.o.asm.d",--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--gdwarf-2,--defsym=__DEBUG=1,--defsym=PKOBSKDEPlatformTool=1
	
${OBJECTDIR}/change_notif.o: change_notif.S  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/change_notif.o.d 
	@${RM} ${OBJECTDIR}/change_notif.o 
	@${RM} ${OBJECTDIR}/change_notif.o.ok ${OBJECTDIR}/change_notif.o.err 
	@${FIXDEPS} "${OBJECTDIR}/change_notif.o.d" "${OBJECTDIR}/change_notif.o.asm.d" -t $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC} $(MP_EXTRA_AS_PRE)  -D__DEBUG -DPKOBSKDEPlatformTool=1 -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/change_notif.o.d"  -o ${OBJECTDIR}/change_notif.o change_notif.S -relaxed-math -DXPRJ_default=$(CND_CONF)  -legacy-libc  -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),-MD="${OBJECTDIR}/change_notif.o.asm.d",--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--gdwarf-2,--defsym=__DEBUG=1,--defsym=PKOBSKDEPlatformTool=1
	
${OBJECTDIR}/ubtn2.o: ubtn2.S  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/ubtn2.o.d 
	@${RM} ${OBJECTDIR}/ubtn2.o 
	@${RM} ${OBJECTDIR}/ubtn2.o.ok ${OBJECTDIR}/ubtn2.o.err 
	@${FIXDEPS} "${OBJECTDIR}/ubtn2.o.d" "${OBJECTDIR}/ubtn2.o.asm.d" -t $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC} $(MP_EXTRA_AS_PRE)  -D__DEBUG -DPKOBSKDEPlatformTool=1 -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/ubtn2.o.d"  -o ${OBJECTDIR}/ubtn2.o ubtn2.S -relaxed-math -DXPRJ_default=$(CND_CONF)  -legacy-libc  -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),-MD="${OBJECTDIR}/ubtn2.o.asm.d",--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--gdwarf-2,--defsym=__DEBUG=1,--defsym=PKOBSKDEPlatformTool=1
	
${OBJECTDIR}/periph_fn.o: periph_fn.S  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/periph_fn.o.d 
	@${RM} ${OBJECTDIR}/periph_fn.o 
	@${RM} ${OBJECTDIR}/periph_fn.o.ok ${OBJECTDIR}/periph_fn.o.err 
	@${FIXDEPS} "${OBJECTDIR}/periph_fn.o.d" "${OBJECTDIR}/periph_fn.o.asm.d" -t $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC} $(MP_EXTRA_AS_PRE)  -D__DEBUG -DPKOBSKDEPlatformTool=1 -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/periph_fn.o.d"  -o ${OBJECTDIR}/periph_fn.o periph_fn.S -relaxed-math -DXPRJ_default=$(CND_CONF)  -legacy-libc  -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),-MD="${OBJECTDIR}/periph_fn.o.asm.d",--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--gdwarf-2,--defsym=__DEBUG=1,--defsym=PKOBSKDEPlatformTool=1
	
else
${OBJECTDIR}/asm_macro.o: asm_macro.S  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/asm_macro.o.d 
	@${RM} ${OBJECTDIR}/asm_macro.o 
	@${RM} ${OBJECTDIR}/asm_macro.o.ok ${OBJECTDIR}/asm_macro.o.err 
	@${FIXDEPS} "${OBJECTDIR}/asm_macro.o.d" "${OBJECTDIR}/asm_macro.o.asm.d" -t $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC} $(MP_EXTRA_AS_PRE)  -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/asm_macro.o.d"  -o ${OBJECTDIR}/asm_macro.o asm_macro.S -relaxed-math -DXPRJ_default=$(CND_CONF)  -legacy-libc  -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),-MD="${OBJECTDIR}/asm_macro.o.asm.d",--gdwarf-2
	
${OBJECTDIR}/timer1.o: timer1.S  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/timer1.o.d 
	@${RM} ${OBJECTDIR}/timer1.o 
	@${RM} ${OBJECTDIR}/timer1.o.ok ${OBJECTDIR}/timer1.o.err 
	@${FIXDEPS} "${OBJECTDIR}/timer1.o.d" "${OBJECTDIR}/timer1.o.asm.d" -t $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC} $(MP_EXTRA_AS_PRE)  -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/timer1.o.d"  -o ${OBJECTDIR}/timer1.o timer1.S -relaxed-math -DXPRJ_default=$(CND_CONF)  -legacy-libc  -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),-MD="${OBJECTDIR}/timer1.o.asm.d",--gdwarf-2
	
${OBJECTDIR}/core_fn.o: core_fn.S  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/core_fn.o.d 
	@${RM} ${OBJECTDIR}/core_fn.o 
	@${RM} ${OBJECTDIR}/core_fn.o.ok ${OBJECTDIR}/core_fn.o.err 
	@${FIXDEPS} "${OBJECTDIR}/core_fn.o.d" "${OBJECTDIR}/core_fn.o.asm.d" -t $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC} $(MP_EXTRA_AS_PRE)  -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/core_fn.o.d"  -o ${OBJECTDIR}/core_fn.o core_fn.S -relaxed-math -DXPRJ_default=$(CND_CONF)  -legacy-libc  -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),-MD="${OBJECTDIR}/core_fn.o.asm.d",--gdwarf-2
	
${OBJECTDIR}/rtc.o: rtc.S  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/rtc.o.d 
	@${RM} ${OBJECTDIR}/rtc.o 
	@${RM} ${OBJECTDIR}/rtc.o.ok ${OBJECTDIR}/rtc.o.err 
	@${FIXDEPS} "${OBJECTDIR}/rtc.o.d" "${OBJECTDIR}/rtc.o.asm.d" -t $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC} $(MP_EXTRA_AS_PRE)  -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/rtc.o.d"  -o ${OBJECTDIR}/rtc.o rtc.S -relaxed-math -DXPRJ_default=$(CND_CONF)  -legacy-libc  -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),-MD="${OBJECTDIR}/rtc.o.asm.d",--gdwarf-2
	
${OBJECTDIR}/change_notif.o: change_notif.S  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/change_notif.o.d 
	@${RM} ${OBJECTDIR}/change_notif.o 
	@${RM} ${OBJECTDIR}/change_notif.o.ok ${OBJECTDIR}/change_notif.o.err 
	@${FIXDEPS} "${OBJECTDIR}/change_notif.o.d" "${OBJECTDIR}/change_notif.o.asm.d" -t $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC} $(MP_EXTRA_AS_PRE)  -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/change_notif.o.d"  -o ${OBJECTDIR}/change_notif.o change_notif.S -relaxed-math -DXPRJ_default=$(CND_CONF)  -legacy-libc  -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),-MD="${OBJECTDIR}/change_notif.o.asm.d",--gdwarf-2
	
${OBJECTDIR}/ubtn2.o: ubtn2.S  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/ubtn2.o.d 
	@${RM} ${OBJECTDIR}/ubtn2.o 
	@${RM} ${OBJECTDIR}/ubtn2.o.ok ${OBJECTDIR}/ubtn2.o.err 
	@${FIXDEPS} "${OBJECTDIR}/ubtn2.o.d" "${OBJECTDIR}/ubtn2.o.asm.d" -t $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC} $(MP_EXTRA_AS_PRE)  -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/ubtn2.o.d"  -o ${OBJECTDIR}/ubtn2.o ubtn2.S -relaxed-math -DXPRJ_default=$(CND_CONF)  -legacy-libc  -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),-MD="${OBJECTDIR}/ubtn2.o.asm.d",--gdwarf-2
	
${OBJECTDIR}/periph_fn.o: periph_fn.S  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/periph_fn.o.d 
	@${RM} ${OBJECTDIR}/periph_fn.o 
	@${RM} ${OBJECTDIR}/periph_fn.o.ok ${OBJECTDIR}/periph_fn.o.err 
	@${FIXDEPS} "${OBJECTDIR}/periph_fn.o.d" "${OBJECTDIR}/periph_fn.o.asm.d" -t $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC} $(MP_EXTRA_AS_PRE)  -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/periph_fn.o.d"  -o ${OBJECTDIR}/periph_fn.o periph_fn.S -relaxed-math -DXPRJ_default=$(CND_CONF)  -legacy-libc  -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),-MD="${OBJECTDIR}/periph_fn.o.asm.d",--gdwarf-2
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/clock_cfgBits_mm.o: clock_cfgBits_mm.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/clock_cfgBits_mm.o.d 
	@${RM} ${OBJECTDIR}/clock_cfgBits_mm.o 
	@${FIXDEPS} "${OBJECTDIR}/clock_cfgBits_mm.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -DPKOBSKDEPlatformTool=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -funroll-loops -fno-schedule-insns -mcci -MMD -MF "${OBJECTDIR}/clock_cfgBits_mm.o.d" -o ${OBJECTDIR}/clock_cfgBits_mm.o clock_cfgBits_mm.c   -relaxed-math -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/clock_cfgBits_mz.o: clock_cfgBits_mz.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/clock_cfgBits_mz.o.d 
	@${RM} ${OBJECTDIR}/clock_cfgBits_mz.o 
	@${FIXDEPS} "${OBJECTDIR}/clock_cfgBits_mz.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -DPKOBSKDEPlatformTool=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -funroll-loops -fno-schedule-insns -mcci -MMD -MF "${OBJECTDIR}/clock_cfgBits_mz.o.d" -o ${OBJECTDIR}/clock_cfgBits_mz.o clock_cfgBits_mz.c   -relaxed-math -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/main.o: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	@${FIXDEPS} "${OBJECTDIR}/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -DPKOBSKDEPlatformTool=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -funroll-loops -fno-schedule-insns -mcci -MMD -MF "${OBJECTDIR}/main.o.d" -o ${OBJECTDIR}/main.o main.c   -relaxed-math -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/globals.o: globals.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/globals.o.d 
	@${RM} ${OBJECTDIR}/globals.o 
	@${FIXDEPS} "${OBJECTDIR}/globals.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -DPKOBSKDEPlatformTool=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -funroll-loops -fno-schedule-insns -mcci -MMD -MF "${OBJECTDIR}/globals.o.d" -o ${OBJECTDIR}/globals.o globals.c   -relaxed-math -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/periph.o: periph.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/periph.o.d 
	@${RM} ${OBJECTDIR}/periph.o 
	@${FIXDEPS} "${OBJECTDIR}/periph.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -DPKOBSKDEPlatformTool=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -funroll-loops -fno-schedule-insns -mcci -MMD -MF "${OBJECTDIR}/periph.o.d" -o ${OBJECTDIR}/periph.o periph.c   -relaxed-math -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/app1.o: app1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/app1.o.d 
	@${RM} ${OBJECTDIR}/app1.o 
	@${FIXDEPS} "${OBJECTDIR}/app1.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -DPKOBSKDEPlatformTool=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -funroll-loops -fno-schedule-insns -mcci -MMD -MF "${OBJECTDIR}/app1.o.d" -o ${OBJECTDIR}/app1.o app1.c   -relaxed-math -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/app2.o: app2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/app2.o.d 
	@${RM} ${OBJECTDIR}/app2.o 
	@${FIXDEPS} "${OBJECTDIR}/app2.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -DPKOBSKDEPlatformTool=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -funroll-loops -fno-schedule-insns -mcci -MMD -MF "${OBJECTDIR}/app2.o.d" -o ${OBJECTDIR}/app2.o app2.c   -relaxed-math -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/app3.o: app3.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/app3.o.d 
	@${RM} ${OBJECTDIR}/app3.o 
	@${FIXDEPS} "${OBJECTDIR}/app3.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -DPKOBSKDEPlatformTool=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -funroll-loops -fno-schedule-insns -mcci -MMD -MF "${OBJECTDIR}/app3.o.d" -o ${OBJECTDIR}/app3.o app3.c   -relaxed-math -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/user_apps.o: user_apps.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/user_apps.o.d 
	@${RM} ${OBJECTDIR}/user_apps.o 
	@${FIXDEPS} "${OBJECTDIR}/user_apps.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -DPKOBSKDEPlatformTool=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -funroll-loops -fno-schedule-insns -mcci -MMD -MF "${OBJECTDIR}/user_apps.o.d" -o ${OBJECTDIR}/user_apps.o user_apps.c   -relaxed-math -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
else
${OBJECTDIR}/clock_cfgBits_mm.o: clock_cfgBits_mm.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/clock_cfgBits_mm.o.d 
	@${RM} ${OBJECTDIR}/clock_cfgBits_mm.o 
	@${FIXDEPS} "${OBJECTDIR}/clock_cfgBits_mm.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -funroll-loops -fno-schedule-insns -mcci -MMD -MF "${OBJECTDIR}/clock_cfgBits_mm.o.d" -o ${OBJECTDIR}/clock_cfgBits_mm.o clock_cfgBits_mm.c   -relaxed-math -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/clock_cfgBits_mz.o: clock_cfgBits_mz.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/clock_cfgBits_mz.o.d 
	@${RM} ${OBJECTDIR}/clock_cfgBits_mz.o 
	@${FIXDEPS} "${OBJECTDIR}/clock_cfgBits_mz.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -funroll-loops -fno-schedule-insns -mcci -MMD -MF "${OBJECTDIR}/clock_cfgBits_mz.o.d" -o ${OBJECTDIR}/clock_cfgBits_mz.o clock_cfgBits_mz.c   -relaxed-math -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
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
	
${OBJECTDIR}/periph.o: periph.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/periph.o.d 
	@${RM} ${OBJECTDIR}/periph.o 
	@${FIXDEPS} "${OBJECTDIR}/periph.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -funroll-loops -fno-schedule-insns -mcci -MMD -MF "${OBJECTDIR}/periph.o.d" -o ${OBJECTDIR}/periph.o periph.c   -relaxed-math -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
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
dist/${CND_CONF}/${IMAGE_TYPE}/BOS2.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -g -mdebugger -DPKOBSKDEPlatformTool=1 -mprocessor=$(MP_PROCESSOR_OPTION) -O3 -o dist/${CND_CONF}/${IMAGE_TYPE}/BOS2.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}         -relaxed-math -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)   -mreserve=data@0x0:0x27F   -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D=__DEBUG_D,--defsym=PKOBSKDEPlatformTool=1,--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/BOS2.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION) -O3 -o dist/${CND_CONF}/${IMAGE_TYPE}/BOS2.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}         -relaxed-math -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml
	${MP_CC_DIR}\\xc32-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/BOS2.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} 
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
