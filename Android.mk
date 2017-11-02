INNER_SAVED_LOCAL_PATH := $(LOCAL_PATH)

LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := sprite2
# LOCAL_CPPFLAGS += -frtti

LOCAL_C_INCLUDES := \
	${CLIB_PATH} \
	${SPRITE2_SRC_PATH}/include \
	${SPRITE2_SRC_PATH}/include \
	${SM_SRC_PATH} \
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
	${FLATTEN_SRC_PATH}/include \
	${MEMMGR_SRC_PATH}/include \

LOCAL_SRC_FILES := \
	$(subst $(LOCAL_PATH)/,,$(shell find $(LOCAL_PATH) -name "*.cpp" -print)) \

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
	flatten \

include $(BUILD_STATIC_LIBRARY)	

LOCAL_PATH := $(INNER_SAVED_LOCAL_PATH)