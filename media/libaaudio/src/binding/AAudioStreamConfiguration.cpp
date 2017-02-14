/*
 * Copyright 2016 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdint.h>

#include <sys/mman.h>
#include <binder/Parcel.h>
#include <binder/Parcelable.h>

#include <aaudio/AAudioDefinitions.h>

#include "binding/AAudioStreamConfiguration.h"

using android::NO_ERROR;
using android::status_t;
using android::Parcel;
using android::Parcelable;

using namespace aaudio;

AAudioStreamConfiguration::AAudioStreamConfiguration() {}
AAudioStreamConfiguration::~AAudioStreamConfiguration() {}

status_t AAudioStreamConfiguration::writeToParcel(Parcel* parcel) const {
    parcel->writeInt32(mDeviceId);
    parcel->writeInt32(mSampleRate);
    parcel->writeInt32(mSamplesPerFrame);
    parcel->writeInt32((int32_t) mAudioFormat);
    parcel->writeInt32(mBufferCapacity);
    return NO_ERROR; // TODO check for errors above
}

status_t AAudioStreamConfiguration::readFromParcel(const Parcel* parcel) {
    int32_t temp;
    parcel->readInt32(&mDeviceId);
    parcel->readInt32(&mSampleRate);
    parcel->readInt32(&mSamplesPerFrame);
    parcel->readInt32(&temp);
    mAudioFormat = (aaudio_audio_format_t) temp;
    parcel->readInt32(&mBufferCapacity);
    return NO_ERROR; // TODO check for errors above
}

aaudio_result_t AAudioStreamConfiguration::validate() {
    // Validate results of the open.
    if (mSampleRate < 0 || mSampleRate >= 8 * 48000) { // TODO review limits
        ALOGE("AAudioStreamConfiguration.validate(): invalid sampleRate = %d", mSampleRate);
        return AAUDIO_ERROR_INTERNAL;
    }

    if (mSamplesPerFrame < 1 || mSamplesPerFrame >= 32) { // TODO review limits
        ALOGE("AAudioStreamConfiguration.validate() invalid samplesPerFrame = %d", mSamplesPerFrame);
        return AAUDIO_ERROR_INTERNAL;
    }

    switch (mAudioFormat) {
    case AAUDIO_FORMAT_PCM_I16:
    case AAUDIO_FORMAT_PCM_FLOAT:
    case AAUDIO_FORMAT_PCM_I8_24:
    case AAUDIO_FORMAT_PCM_I32:
        break;
    default:
        ALOGE("AAudioStreamConfiguration.validate() invalid audioFormat = %d", mAudioFormat);
        return AAUDIO_ERROR_INTERNAL;
    }

    if (mBufferCapacity < 0) {
        ALOGE("AAudioStreamConfiguration.validate() invalid mBufferCapacity = %d", mBufferCapacity);
        return AAUDIO_ERROR_INTERNAL;
    }
    return AAUDIO_OK;
}

void AAudioStreamConfiguration::dump() {
    ALOGD("AAudioStreamConfiguration mSampleRate      = %d -----", mSampleRate);
    ALOGD("AAudioStreamConfiguration mSamplesPerFrame = %d", mSamplesPerFrame);
    ALOGD("AAudioStreamConfiguration mAudioFormat     = %d", (int)mAudioFormat);
    ALOGD("AAudioStreamConfiguration mBufferCapacity  = %d", mBufferCapacity);
}