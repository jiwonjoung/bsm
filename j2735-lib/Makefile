default: libbsm.a

rebuild:
	$(MAKE) clean
	$(MAKE) regen-makefile
	$(MAKE)
	$(MAKE) clean
	$(MAKE) mk5

CFLAGS+=-Werror -Wall -I.. -I../../../../ieee1609/kernel/include/ -I../../../../cohda/kernel/include/

# Comment/uncomment the following line to disable/enable debugging
DEBUG = y
ifeq ($(DEBUG),y)
  # "-O" is needed to expand inlines
  CFLAGS += -O -g -DDEBUG 
else
CFLAGS += -O2
endif

# If using yocto/poky toolchain, use CC in environment
ifneq ($(TARGET_PREFIX),arm-poky-linux-gnueabi-)
  CC = $(CROSS_COMPILE)gcc
endif
AR = $(CROSS_COMPILE)ar

ASN_MODULE_SOURCES=	\
	DSRC_AlaCarte.c	\
	DSRC_BasicSafetyMessage.c	\
	DSRC_VehicleDataV2VI.c	\
	DSRC_TestDataFrameV2VI.c	\
	DSRC_BasicSafetyMessageVerbose.c	\
	DSRC_CommonSafetyRequest.c	\
	DSRC_EmergencyVehicleAlert.c	\
	DSRC_IntersectionCollision.c	\
	DSRC_MapData.c	\
	DSRC_NMEA-Corrections.c	\
	DSRC_ProbeDataManagement.c	\
	DSRC_ProbeVehicleData.c	\
	DSRC_RoadSideAlert.c	\
	DSRC_RTCM-Corrections.c	\
	DSRC_SignalRequestMsg.c	\
	DSRC_SignalStatusMessage.c	\
	DSRC_SPAT.c	\
	DSRC_TravelerInformation.c	\
	DSRC_AccelerationSet4Way.c	\
	DSRC_AccelSteerYawRateConfidence.c	\
	DSRC_AllInclusive.c	\
	DSRC_AntennaOffsetSet.c	\
	DSRC_Approach.c	\
	DSRC_ApproachObject.c	\
	DSRC_BarrierLane.c	\
	DSRC_BrakeSystemStatus.c	\
	DSRC_BSMblob.c	\
	DSRC_BumperHeights.c	\
	DSRC_Circle.c	\
	DSRC_ConfidenceSet.c	\
	DSRC_ConnectsTo.c	\
	DSRC_CrosswalkLane.c	\
	DSRC_DataParameters.c	\
	DSRC_DDate.c	\
	DSRC_DDateTime.c	\
	DSRC_DFullTime.c	\
	DSRC_DMonthDay.c	\
	DSRC_DTime.c	\
	DSRC_DYearMonth.c	\
	DSRC_ExitService.c	\
	DSRC_FullPositionVector.c	\
	DSRC_GenericSignage.c	\
	DSRC_Intersection.c	\
	DSRC_IntersectionState.c	\
	DSRC_J1939data.c	\
	DSRC_MovementState.c	\
	DSRC_NodeList.c	\
	DSRC_Offsets.c	\
	DSRC_PathHistory.c	\
	DSRC_PathHistoryPointType-01.c	\
	DSRC_PathHistoryPointType-02.c	\
	DSRC_PathHistoryPointType-03.c	\
	DSRC_PathHistoryPointType-04.c	\
	DSRC_PathHistoryPointType-05.c	\
	DSRC_PathHistoryPointType-06.c	\
	DSRC_PathHistoryPointType-07.c	\
	DSRC_PathHistoryPointType-08.c	\
	DSRC_PathHistoryPointType-09.c	\
	DSRC_PathHistoryPointType-10.c	\
	DSRC_PathPrediction.c	\
	DSRC_Position3D.c	\
	DSRC_PositionalAccuracy.c	\
	DSRC_PositionConfidenceSet.c	\
	DSRC_RegionList.c	\
	DSRC_RegionOffsets.c	\
	DSRC_RegionPointSet.c	\
	DSRC_RoadSignID.c	\
	DSRC_RTCMHeader.c	\
	DSRC_RTCMmsg.c	\
	DSRC_RTCMPackage.c	\
	DSRC_Sample.c	\
	DSRC_ShapePointSet.c	\
	DSRC_SignalControlZone.c	\
	DSRC_SignalRequest.c	\
	DSRC_Snapshot.c	\
	DSRC_SnapshotDistance.c	\
	DSRC_SnapshotTime.c	\
	DSRC_SpecialLane.c	\
	DSRC_SpeedandHeadingandThrottleConfidence.c	\
	DSRC_SpeedLimit.c	\
	DSRC_TransmissionAndSpeed.c	\
	DSRC_ValidRegion.c	\
	DSRC_VehicleComputedLane.c	\
	DSRC_VehicleIdent.c	\
	DSRC_VehicleReferenceLane.c	\
	DSRC_VehicleSafetyExtension.c	\
	DSRC_VehicleSize.c	\
	DSRC_VehicleStatus.c	\
	DSRC_VehicleStatusRequest.c	\
	DSRC_WiperStatus.c	\
	DSRC_WorkZone.c	\
	DSRC_Acceleration.c	\
	DSRC_AccelerationConfidence.c	\
	DSRC_AmbientAirPressure.c	\
	DSRC_AmbientAirTemperature.c	\
	DSRC_AntiLockBrakeStatus.c	\
	DSRC_ApproachNumber.c	\
	DSRC_AuxiliaryBrakeStatus.c	\
	DSRC_AxleLocation.c	\
	DSRC_AxleWeight.c	\
	DSRC_BarrierAttributes.c	\
	DSRC_BrakeAppliedPressure.c	\
	DSRC_BrakeAppliedStatus.c	\
	DSRC_BrakeBoostApplied.c	\
	DSRC_BumperHeightFront.c	\
	DSRC_BumperHeightRear.c	\
	DSRC_CargoWeight.c	\
	DSRC_CodeWord.c	\
	DSRC_CoefficientOfFriction.c	\
	DSRC_ColorState.c	\
	DSRC_Count.c	\
	DSRC_CrosswalkLaneAttributes.c	\
	DSRC_DDay.c	\
	DSRC_DescriptiveName.c	\
	DSRC_DHour.c	\
	DSRC_DirectionOfUse.c	\
	DSRC_DMinute.c	\
	DSRC_DMonth.c	\
	DSRC_DOffset.c	\
	DSRC_DriveAxleLiftAirPressure.c	\
	DSRC_DriveAxleLocation.c	\
	DSRC_DriveAxleLubePressure.c	\
	DSRC_DriveAxleTemperature.c	\
	DSRC_DrivenLineOffset.c	\
	DSRC_DrivingWheelAngle.c	\
	DSRC_DSecond.c	\
	DSRC_DSignalSeconds.c	\
	DSRC_DSRCmsgID.c	\
	DSRC_DYear.c	\
	DSRC_Elevation.c	\
	DSRC_ElevationConfidence.c	\
	DSRC_EmergencyDetails.c	\
	DSRC_EventFlags.c	\
	DSRC_Extent.c	\
	DSRC_ExteriorLights.c	\
	DSRC_FurtherInfoID.c	\
	DSRC_GPSstatus.c	\
	DSRC_Heading.c	\
	DSRC_HeadingConfidence.c	\
	DSRC_HeadingSlice.c	\
	DSRC_IntersectionID.c	\
	DSRC_IntersectionStatusObject.c	\
	DSRC_LaneCount.c	\
	DSRC_LaneManeuverCode.c	\
	DSRC_LaneNumber.c	\
	DSRC_LaneSet.c	\
	DSRC_LaneWidth.c	\
	DSRC_Latitude.c	\
	DSRC_LayerID.c	\
	DSRC_LayerType.c	\
	DSRC_LightbarInUse.c	\
	DSRC_Location-quality.c	\
	DSRC_Location-tech.c	\
	DSRC_Longitude.c	\
	DSRC_MinuteOfTheYear.c	\
	DSRC_MinutesDuration.c	\
	DSRC_MsgCount.c	\
	DSRC_MsgCRC.c	\
	DSRC_MultiVehicleResponse.c	\
	DSRC_MUTCDCode.c	\
	DSRC_NMEA-MsgType.c	\
	DSRC_NMEA-Payload.c	\
	DSRC_NMEA-Revision.c	\
	DSRC_NTCIPVehicleclass.c	\
	DSRC_ObjectCount.c	\
	DSRC_ObstacleDirection.c	\
	DSRC_ObstacleDistance.c	\
	DSRC_Payload.c	\
	DSRC_PayloadData.c	\
	DSRC_PedestrianDetect.c	\
	DSRC_PedestrianSignalState.c	\
	DSRC_PositionConfidence.c	\
	DSRC_PreemptState.c	\
	DSRC_Priority.c	\
	DSRC_PriorityState.c	\
	DSRC_ProbeSegmentNumber.c	\
	DSRC_RainSensor.c	\
	DSRC_RequestedItem.c	\
	DSRC_ResponseType.c	\
	DSRC_RTCM-ID.c	\
	DSRC_RTCM-Payload.c	\
	DSRC_RTCM-Revision.c	\
	DSRC_SignalLightState.c	\
	DSRC_SignalReqScheme.c	\
	DSRC_SignalState.c	\
	DSRC_SignPrority.c	\
	DSRC_SirenInUse.c	\
	DSRC_SpecialLaneAttributes.c	\
	DSRC_SpecialSignalState.c	\
	DSRC_Speed.c	\
	DSRC_SpeedConfidence.c	\
	DSRC_StabilityControlStatus.c	\
	DSRC_StateConfidence.c	\
	DSRC_SteeringAxleLubePressure.c	\
	DSRC_SteeringAxleTemperature.c	\
	DSRC_SteeringWheelAngle.c	\
	DSRC_SteeringWheelAngleConfidence.c	\
	DSRC_SteeringWheelAngleRateOfChange.c	\
	DSRC_SunSensor.c	\
	DSRC_TemporaryID.c	\
	DSRC_TermDistance.c	\
	DSRC_TermTime.c	\
	DSRC_ThrottleConfidence.c	\
	DSRC_ThrottlePosition.c	\
	DSRC_TimeConfidence.c	\
	DSRC_TimeMark.c	\
	DSRC_TireLeakageRate.c	\
	DSRC_TireLocation.c	\
	DSRC_TirePressure.c	\
	DSRC_TirePressureThresholdDetection.c	\
	DSRC_TireTemp.c	\
	DSRC_TractionControlState.c	\
	DSRC_TrailerWeight.c	\
	DSRC_TransitPreEmptionRequest.c	\
	DSRC_TransitStatus.c	\
	DSRC_TransmissionState.c	\
	DSRC_TravelerInfoType.c	\
	DSRC_TxTime.c	\
	DSRC_UniqueMSGID.c	\
	DSRC_URL-Base.c	\
	DSRC_URL-Link.c	\
	DSRC_URL-Short.c	\
	DSRC_VehicleHeight.c	\
	DSRC_VehicleLaneAttributes.c	\
	DSRC_VehicleLength.c	\
	DSRC_VehicleMass.c	\
	DSRC_VehicleRequestStatus.c	\
	DSRC_VehicleStatusDeviceTypeTag.c	\
	DSRC_VehicleType.c	\
	DSRC_VehicleWidth.c	\
	DSRC_VerticalAcceleration.c	\
	DSRC_VerticalAccelerationThreshold.c	\
	DSRC_VINstring.c	\
	DSRC_WheelEndElectFault.c	\
	DSRC_WheelSensorStatus.c	\
	DSRC_WiperRate.c	\
	DSRC_WiperStatusFront.c	\
	DSRC_WiperStatusRear.c	\
	DSRC_YawRate.c	\
	DSRC_YawRateConfidence.c	\
	DSRC_EssMobileFriction.c	\
	DSRC_EssPrecipRate.c	\
	DSRC_EssPrecipSituation.c	\
	DSRC_EssPrecipYesNo.c	\
	DSRC_EssSolarRadiation.c	\
	DSRC_IncidentResponseEquipment.c	\
	DSRC_ITIScodes.c	\
	DSRC_ITIScodesAndText.c	\
	DSRC_ITIStext.c	\
	DSRC_ResponderGroupAffected.c	\
	DSRC_VehicleGroupAffected.c

ASN_MODULE_HEADERS=	\
	DSRC_AlaCarte.h	\
	DSRC_BasicSafetyMessage.h	\
	DSRC_VehicleDataV2VI.h	\
	DSRC_TestDataFrameV2VI.h	\
	DSRC_BasicSafetyMessageVerbose.h	\
	DSRC_CommonSafetyRequest.h	\
	DSRC_EmergencyVehicleAlert.h	\
	DSRC_IntersectionCollision.h	\
	DSRC_MapData.h	\
	DSRC_NMEA-Corrections.h	\
	DSRC_ProbeDataManagement.h	\
	DSRC_ProbeVehicleData.h	\
	DSRC_RoadSideAlert.h	\
	DSRC_RTCM-Corrections.h	\
	DSRC_SignalRequestMsg.h	\
	DSRC_SignalStatusMessage.h	\
	DSRC_SPAT.h	\
	DSRC_TravelerInformation.h	\
	DSRC_AccelerationSet4Way.h	\
	DSRC_AccelSteerYawRateConfidence.h	\
	DSRC_AllInclusive.h	\
	DSRC_AntennaOffsetSet.h	\
	DSRC_Approach.h	\
	DSRC_ApproachObject.h	\
	DSRC_BarrierLane.h	\
	DSRC_BrakeSystemStatus.h	\
	DSRC_BSMblob.h	\
	DSRC_BumperHeights.h	\
	DSRC_Circle.h	\
	DSRC_ConfidenceSet.h	\
	DSRC_ConnectsTo.h	\
	DSRC_CrosswalkLane.h	\
	DSRC_DataParameters.h	\
	DSRC_DDate.h	\
	DSRC_DDateTime.h	\
	DSRC_DFullTime.h	\
	DSRC_DMonthDay.h	\
	DSRC_DTime.h	\
	DSRC_DYearMonth.h	\
	DSRC_ExitService.h	\
	DSRC_FullPositionVector.h	\
	DSRC_GenericSignage.h	\
	DSRC_Intersection.h	\
	DSRC_IntersectionState.h	\
	DSRC_J1939data.h	\
	DSRC_MovementState.h	\
	DSRC_NodeList.h	\
	DSRC_Offsets.h	\
	DSRC_PathHistory.h	\
	DSRC_PathHistoryPointType-01.h	\
	DSRC_PathHistoryPointType-02.h	\
	DSRC_PathHistoryPointType-03.h	\
	DSRC_PathHistoryPointType-04.h	\
	DSRC_PathHistoryPointType-05.h	\
	DSRC_PathHistoryPointType-06.h	\
	DSRC_PathHistoryPointType-07.h	\
	DSRC_PathHistoryPointType-08.h	\
	DSRC_PathHistoryPointType-09.h	\
	DSRC_PathHistoryPointType-10.h	\
	DSRC_PathPrediction.h	\
	DSRC_Position3D.h	\
	DSRC_PositionalAccuracy.h	\
	DSRC_PositionConfidenceSet.h	\
	DSRC_RegionList.h	\
	DSRC_RegionOffsets.h	\
	DSRC_RegionPointSet.h	\
	DSRC_RoadSignID.h	\
	DSRC_RTCMHeader.h	\
	DSRC_RTCMmsg.h	\
	DSRC_RTCMPackage.h	\
	DSRC_Sample.h	\
	DSRC_ShapePointSet.h	\
	DSRC_SignalControlZone.h	\
	DSRC_SignalRequest.h	\
	DSRC_Snapshot.h	\
	DSRC_SnapshotDistance.h	\
	DSRC_SnapshotTime.h	\
	DSRC_SpecialLane.h	\
	DSRC_SpeedandHeadingandThrottleConfidence.h	\
	DSRC_SpeedLimit.h	\
	DSRC_TransmissionAndSpeed.h	\
	DSRC_ValidRegion.h	\
	DSRC_VehicleComputedLane.h	\
	DSRC_VehicleIdent.h	\
	DSRC_VehicleReferenceLane.h	\
	DSRC_VehicleSafetyExtension.h	\
	DSRC_VehicleSize.h	\
	DSRC_VehicleStatus.h	\
	DSRC_VehicleStatusRequest.h	\
	DSRC_WiperStatus.h	\
	DSRC_WorkZone.h	\
	DSRC_Acceleration.h	\
	DSRC_AccelerationConfidence.h	\
	DSRC_AmbientAirPressure.h	\
	DSRC_AmbientAirTemperature.h	\
	DSRC_AntiLockBrakeStatus.h	\
	DSRC_ApproachNumber.h	\
	DSRC_AuxiliaryBrakeStatus.h	\
	DSRC_AxleLocation.h	\
	DSRC_AxleWeight.h	\
	DSRC_BarrierAttributes.h	\
	DSRC_BrakeAppliedPressure.h	\
	DSRC_BrakeAppliedStatus.h	\
	DSRC_BrakeBoostApplied.h	\
	DSRC_BumperHeightFront.h	\
	DSRC_BumperHeightRear.h	\
	DSRC_CargoWeight.h	\
	DSRC_CodeWord.h	\
	DSRC_CoefficientOfFriction.h	\
	DSRC_ColorState.h	\
	DSRC_Count.h	\
	DSRC_CrosswalkLaneAttributes.h	\
	DSRC_DDay.h	\
	DSRC_DescriptiveName.h	\
	DSRC_DHour.h	\
	DSRC_DirectionOfUse.h	\
	DSRC_DMinute.h	\
	DSRC_DMonth.h	\
	DSRC_DOffset.h	\
	DSRC_DriveAxleLiftAirPressure.h	\
	DSRC_DriveAxleLocation.h	\
	DSRC_DriveAxleLubePressure.h	\
	DSRC_DriveAxleTemperature.h	\
	DSRC_DrivenLineOffset.h	\
	DSRC_DrivingWheelAngle.h	\
	DSRC_DSecond.h	\
	DSRC_DSignalSeconds.h	\
	DSRC_DSRCmsgID.h	\
	DSRC_DYear.h	\
	DSRC_Elevation.h	\
	DSRC_ElevationConfidence.h	\
	DSRC_EmergencyDetails.h	\
	DSRC_EventFlags.h	\
	DSRC_Extent.h	\
	DSRC_ExteriorLights.h	\
	DSRC_FurtherInfoID.h	\
	DSRC_GPSstatus.h	\
	DSRC_Heading.h	\
	DSRC_HeadingConfidence.h	\
	DSRC_HeadingSlice.h	\
	DSRC_IntersectionID.h	\
	DSRC_IntersectionStatusObject.h	\
	DSRC_LaneCount.h	\
	DSRC_LaneManeuverCode.h	\
	DSRC_LaneNumber.h	\
	DSRC_LaneSet.h	\
	DSRC_LaneWidth.h	\
	DSRC_Latitude.h	\
	DSRC_LayerID.h	\
	DSRC_LayerType.h	\
	DSRC_LightbarInUse.h	\
	DSRC_Location-quality.h	\
	DSRC_Location-tech.h	\
	DSRC_Longitude.h	\
	DSRC_MinuteOfTheYear.h	\
	DSRC_MinutesDuration.h	\
	DSRC_MsgCount.h	\
	DSRC_MsgCRC.h	\
	DSRC_MultiVehicleResponse.h	\
	DSRC_MUTCDCode.h	\
	DSRC_NMEA-MsgType.h	\
	DSRC_NMEA-Payload.h	\
	DSRC_NMEA-Revision.h	\
	DSRC_NTCIPVehicleclass.h	\
	DSRC_ObjectCount.h	\
	DSRC_ObstacleDirection.h	\
	DSRC_ObstacleDistance.h	\
	DSRC_Payload.h	\
	DSRC_PayloadData.h	\
	DSRC_PedestrianDetect.h	\
	DSRC_PedestrianSignalState.h	\
	DSRC_PositionConfidence.h	\
	DSRC_PreemptState.h	\
	DSRC_Priority.h	\
	DSRC_PriorityState.h	\
	DSRC_ProbeSegmentNumber.h	\
	DSRC_RainSensor.h	\
	DSRC_RequestedItem.h	\
	DSRC_ResponseType.h	\
	DSRC_RTCM-ID.h	\
	DSRC_RTCM-Payload.h	\
	DSRC_RTCM-Revision.h	\
	DSRC_SignalLightState.h	\
	DSRC_SignalReqScheme.h	\
	DSRC_SignalState.h	\
	DSRC_SignPrority.h	\
	DSRC_SirenInUse.h	\
	DSRC_SpecialLaneAttributes.h	\
	DSRC_SpecialSignalState.h	\
	DSRC_Speed.h	\
	DSRC_SpeedConfidence.h	\
	DSRC_StabilityControlStatus.h	\
	DSRC_StateConfidence.h	\
	DSRC_SteeringAxleLubePressure.h	\
	DSRC_SteeringAxleTemperature.h	\
	DSRC_SteeringWheelAngle.h	\
	DSRC_SteeringWheelAngleConfidence.h	\
	DSRC_SteeringWheelAngleRateOfChange.h	\
	DSRC_SunSensor.h	\
	DSRC_TemporaryID.h	\
	DSRC_TermDistance.h	\
	DSRC_TermTime.h	\
	DSRC_ThrottleConfidence.h	\
	DSRC_ThrottlePosition.h	\
	DSRC_TimeConfidence.h	\
	DSRC_TimeMark.h	\
	DSRC_TireLeakageRate.h	\
	DSRC_TireLocation.h	\
	DSRC_TirePressure.h	\
	DSRC_TirePressureThresholdDetection.h	\
	DSRC_TireTemp.h	\
	DSRC_TractionControlState.h	\
	DSRC_TrailerWeight.h	\
	DSRC_TransitPreEmptionRequest.h	\
	DSRC_TransitStatus.h	\
	DSRC_TransmissionState.h	\
	DSRC_TravelerInfoType.h	\
	DSRC_TxTime.h	\
	DSRC_UniqueMSGID.h	\
	DSRC_URL-Base.h	\
	DSRC_URL-Link.h	\
	DSRC_URL-Short.h	\
	DSRC_VehicleHeight.h	\
	DSRC_VehicleLaneAttributes.h	\
	DSRC_VehicleLength.h	\
	DSRC_VehicleMass.h	\
	DSRC_VehicleRequestStatus.h	\
	DSRC_VehicleStatusDeviceTypeTag.h	\
	DSRC_VehicleType.h	\
	DSRC_VehicleWidth.h	\
	DSRC_VerticalAcceleration.h	\
	DSRC_VerticalAccelerationThreshold.h	\
	DSRC_VINstring.h	\
	DSRC_WheelEndElectFault.h	\
	DSRC_WheelSensorStatus.h	\
	DSRC_WiperRate.h	\
	DSRC_WiperStatusFront.h	\
	DSRC_WiperStatusRear.h	\
	DSRC_YawRate.h	\
	DSRC_YawRateConfidence.h	\
	DSRC_EssMobileFriction.h	\
	DSRC_EssPrecipRate.h	\
	DSRC_EssPrecipSituation.h	\
	DSRC_EssPrecipYesNo.h	\
	DSRC_EssSolarRadiation.h	\
	DSRC_IncidentResponseEquipment.h	\
	DSRC_ITIScodes.h	\
	DSRC_ITIScodesAndText.h	\
	DSRC_ITIStext.h	\
	DSRC_ResponderGroupAffected.h	\
	DSRC_VehicleGroupAffected.h

ASN_MODULE_HEADERS+=BOOLEAN.h
ASN_MODULE_SOURCES+=BOOLEAN.c
ASN_MODULE_HEADERS+=INTEGER.h
ASN_MODULE_HEADERS+=NativeEnumerated.h
ASN_MODULE_HEADERS+=IA5String.h
ASN_MODULE_SOURCES+=IA5String.c
ASN_MODULE_SOURCES+=INTEGER.c
ASN_MODULE_SOURCES+=NativeEnumerated.c
ASN_MODULE_HEADERS+=NativeInteger.h
ASN_MODULE_SOURCES+=NativeInteger.c
ASN_MODULE_HEADERS+=asn_SEQUENCE_OF.h
ASN_MODULE_SOURCES+=asn_SEQUENCE_OF.c
ASN_MODULE_HEADERS+=asn_SET_OF.h
ASN_MODULE_SOURCES+=asn_SET_OF.c
ASN_MODULE_HEADERS+=constr_CHOICE.h
ASN_MODULE_SOURCES+=constr_CHOICE.c
ASN_MODULE_HEADERS+=constr_SEQUENCE.h
ASN_MODULE_SOURCES+=constr_SEQUENCE.c
ASN_MODULE_HEADERS+=constr_SEQUENCE_OF.h
ASN_MODULE_SOURCES+=constr_SEQUENCE_OF.c
ASN_MODULE_HEADERS+=constr_SET_OF.h
ASN_MODULE_SOURCES+=constr_SET_OF.c
ASN_MODULE_HEADERS+=asn_application.h
ASN_MODULE_HEADERS+=asn_system.h
ASN_MODULE_HEADERS+=asn_codecs.h
ASN_MODULE_HEADERS+=asn_internal.h
ASN_MODULE_HEADERS+=OCTET_STRING.h
ASN_MODULE_SOURCES+=OCTET_STRING.c
ASN_MODULE_HEADERS+=BIT_STRING.h
ASN_MODULE_SOURCES+=BIT_STRING.c
ASN_MODULE_SOURCES+=asn_codecs_prim.c
ASN_MODULE_HEADERS+=asn_codecs_prim.h
ASN_MODULE_HEADERS+=ber_tlv_length.h
ASN_MODULE_SOURCES+=ber_tlv_length.c
ASN_MODULE_HEADERS+=ber_tlv_tag.h
ASN_MODULE_SOURCES+=ber_tlv_tag.c
ASN_MODULE_HEADERS+=ber_decoder.h
ASN_MODULE_SOURCES+=ber_decoder.c
ASN_MODULE_HEADERS+=der_encoder.h
ASN_MODULE_SOURCES+=der_encoder.c
ASN_MODULE_HEADERS+=constr_TYPE.h
ASN_MODULE_SOURCES+=constr_TYPE.c
ASN_MODULE_HEADERS+=constraints.h
ASN_MODULE_SOURCES+=constraints.c
ASN_MODULE_HEADERS+=xer_support.h
ASN_MODULE_SOURCES+=xer_support.c
ASN_MODULE_HEADERS+=xer_decoder.h
ASN_MODULE_SOURCES+=xer_decoder.c
ASN_MODULE_HEADERS+=xer_encoder.h
ASN_MODULE_SOURCES+=xer_encoder.c
ASN_MODULE_HEADERS+=per_support.h
ASN_MODULE_SOURCES+=per_support.c
ASN_MODULE_HEADERS+=per_decoder.h
ASN_MODULE_SOURCES+=per_decoder.c
ASN_MODULE_HEADERS+=per_encoder.h
ASN_MODULE_SOURCES+=per_encoder.c
ASN_MODULE_HEADERS+=per_opentype.h
ASN_MODULE_SOURCES+=per_opentype.c
ASN_CONVERTER_SOURCES+=converter-sample.c


lib_LTLIBRARIES=libsomething.la
libsomething_la_SOURCES=$(ASN_MODULE_SOURCES) $(ASN_MODULE_HEADERS)

# This file may be used as an input for make(3)
# Remove the lines below to convert it into a pure .am file
TARGET = dsrc-bsm
CFLAGS += -DHAVE_CONFIG_H  -DPDU=BasicSafetyMessage -I.
OBJS=${ASN_MODULE_SOURCES:.c=.o} ${ASN_CONVERTER_SOURCES:.c=.o}

all: BasicSafetyMessage.c $(TARGET)

$(TARGET): ${OBJS}
	$(CC) $(CFLAGS) -o $(TARGET) ${OBJS} $(LDFLAGS) $(LIBS)

.SUFFIXES:
.SUFFIXES: .c .o

.c.o:
	$(CC) $(CFLAGS) -o $@ -c $<

clean: clean-extra
	rm -f $(TARGET)
	rm -f $(OBJS)

regen: regenerate-from-asn1-source

regenerate-from-asn1-source-ORIG:
	../../../../tools/asn1c/asn1c/asn1c -S ../../../../tools/asn1c/skeletons -pdu=BasicSafetyMessage -fcompound-names DSRC_R36_Source.ASN


LIB_OBJS=${ASN_MODULE_SOURCES:.c=.o}

ARCH ?= $(shell uname --machine)

libbsm.a: $(LIB_OBJS)
	$(AR) rcs $@ $^
	cp $@ libbsm-$(ARCH).a

clean-extra:
	rm -f libbsm.a
	rm -f $(LIB_OBJS)
	rm -f *.orig
	rm -f .Makefile.*.old

regenerate-from-asn1-source: regenerate-from-asn1-source-ORIG mangle

mangle:
	../MangleFiles.py --clobber --verbose -- ../MangleFiles.list

BasicSafetyMessage.c: ../sample.makefile.regen DSRC_R36_Source.ASN
	$(MAKE) regen-makefile
	@touch BasicSafetyMessage.c
	$(MAKE)

regen-makefile:
	cp Makefile .Makefile.$$.old
	ASN1CMDOPTS="-fcompound-names" \
	ASN1MODULES="DSRC_R36_Source.ASN" \
	ASN1PDU=BasicSafetyMessage \
	PROGNAME=dsrc-bsm \
	../sample.makefile.regen
	$(MAKE) mangle

check: ${TARGET} check-ber check-xer check-per
	@echo ================
	@echo All tests passed
	@echo ================

check-ber:
	@if test -f sample-BasicSafetyMessage-1.[db]er ; then \
	for f in sample-BasicSafetyMessage-*.[db]er; do \
	for b in 1 17 33 980 8192; do \
	echo "Recoding $$f into XER and back ($$b)..."; \
	./${TARGET} -b $$b -iber -oxer $$f > ./.tmp.1.$$$$ || exit 2; \
	./${TARGET} -b $$b -ixer -oxer ./.tmp.1.$$$$ > ./.tmp.2.$$$$ || exit 3; \
	diff ./.tmp.1.$$$$ ./.tmp.2.$$$$ || exit 4; \
	rm -f ./.tmp.[12].$$$$; \
	echo "Test junking $$f (please wait)..."; \
	./${TARGET} -J0.0001 -n 1000 -b $$b -iber -onull $$f || exit 5; \
	./${TARGET} -J0.001 -n 1000 -b $$b -iber -onull $$f || exit 6; \
	done; done; fi

check-xer:
	@if test -f sample-BasicSafetyMessage-1.xer ; then \
	for f in sample-BasicSafetyMessage-*.xer; do \
	for b in 1 17 33 980 8192; do \
	echo "Recoding $$f into DER and back ($$b)..."; \
	./${TARGET} -b $$b -ixer -oder $$f > ./.tmp.1.$$$$ || exit 2; \
	./${TARGET} -b $$b -iber -oxer ./.tmp.1.$$$$ > ./.tmp.2.$$$$ || exit 3; \
	diff $$f ./.tmp.2.$$$$ || exit 4; \
	rm -f ./.tmp.[12].$$$$; \
	echo "Test junking $$f (please wait)..."; \
	./${TARGET} -J0.0001 -n 1000 -b $$b -ixer -onull $$f || exit 5; \
	./${TARGET} -J0.001 -n 1000 -b $$b -ixer -onull $$f || exit 6; \
	done; done; fi

check-per:
	@if test -f sample-BasicSafetyMessage-1-nopad.per ; then \
	for f in sample-BasicSafetyMessage-[1-9]-nopad.per; do \
	for b in 1 17 33 980 8192; do \
	echo "Recoding non-padded $$f into DER into XER and back ($$b)..."; \
	./${TARGET} -b $$b -per-nopad -iper -oder $$f > ./.tmp.1.$$$$ || exit 2; \
	./${TARGET} -b $$b -iber -oxer ./.tmp.1.$$$$ > ./.tmp.2.$$$$ || exit 3; \
	./${TARGET} -b $$b -ixer -oder ./.tmp.2.$$$$ > ./.tmp.3.$$$$ || exit 4; \
	diff ./.tmp.1.$$$$ ./.tmp.3.$$$$ || exit 5; \
	rm -f ./.tmp.[123].$$$$; \
	echo "Test junking $$f (please wait)..."; \
	./${TARGET} -J0.0001 -n 1000 -b $$b -per-nopad -iper -onull $$f || exit 6; \
	./${TARGET} -J0.001 -n 1000 -b $$b -per-nopad -iper -onull $$f || exit 7; \
	done; done; fi
	@if test -f sample-BasicSafetyMessage-1.per ; then \
	for f in sample-*-[1-9].per; do \
	pdu=`echo $$f | sed -E -e "s/sample-([A-Za-z-]+)-[0-9].*/\\1/"`; \
	for b in 1 17 33 980 8192; do \
	echo "Recoding $$f into DER into XER and back ($$b)..."; \
	./${TARGET} -b $$b -p $$pdu -iper -oder $$f > ./.tmp.1.$$$$ || exit 3; \
	./${TARGET} -b $$b -p $$pdu -iber -oxer ./.tmp.1.$$$$ > ./.tmp.2.$$$$ || exit 4; \
	./${TARGET} -b $$b -p $$pdu -ixer -oper ./.tmp.2.$$$$ > ./.tmp.1.$$$$ || exit 5; \
	diff $$f ./.tmp.1.$$$$ || exit 6; \
	rm -f ./.tmp.[12].$$$$; \
	echo "Test junking $$f (please wait)..."; \
	./${TARGET} -J0.0001 -n 1000 -b $$b -iper -onull $$f || exit 7; \
	./${TARGET} -J0.001 -n 1000 -b $$b -iper -onull $$f || exit 8; \
	done; done; fi

distclean: clean
	rm -f $(ASN_MODULE_SOURCES)
	rm -f $(ASN_MODULE_HEADERS)
	rm -f $(ASN_CONVERTER_SOURCES) $(ASN_CONVERTER_HEADERS)
	rm -f Makefile.am.sample
	rm -f libbsm*.a

mk5:
	env BOARD=$@ ARCH=arm CROSS_COMPILE="ccache arm-linux-gnueabihf-" $(MAKE)

