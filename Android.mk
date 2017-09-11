INNER_SAVED_LOCAL_PATH := $(LOCAL_PATH)

LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := sprite2
# LOCAL_CPPFLAGS += -frtti

LOCAL_C_INCLUDES := \
	${SPRITE2_SRC_PATH}/include \
	${SPRITE2_SRC_PATH}/include/sprite2 \
	${SM_SRC_PATH} \
	${CU_SRC_PATH} \
	${GTXT_SRC_PATH} \
	${SHADERLAB_SRC_PATH}/include \
	${UNIRENDER_SRC_PATH}/include \
	${UNIAUDIO_SRC_PATH}/include \
	${GIMG_SRC_PATH} \
	${CAMERA25_SRC_PATH} \
	${RVG_SRC_PATH} \
	${LOGGER_SRC_PATH} \
	${RIGGING_SRC_PATH} \
	${POLYMESH_SRC_PATH}/include \
	${PS_SRC_PATH} \
	${MTRAIL_SRC_PATH} \
	${GLP_SRC_PATH} \
	${DS_SRC_PATH} \
	${COOKING_SRC_PATH}/include \
	${MULTITASK_SRC_PATH}/include \

LOCAL_SRC_FILES := \
	$(subst $(LOCAL_PATH)/,,$(shell find $(LOCAL_PATH) -name "*.cpp" -print)) \

LOCAL_CPPFLAGS  := -std=c++11

LOCAL_STATIC_LIBRARIES := \
	sm \
	cu \
	gtxt \
	gimp \
	simp \
	json \
	gimg \
	dtex2 \
	rapidvg \
	polymesh \
	ps \
	mtrail \
	cooking \

include $(BUILD_STATIC_LIBRARY)	

LOCAL_PATH := $(INNER_SAVED_LOCAL_PATH)