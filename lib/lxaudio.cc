#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include "../include/lxaudio.h"
#include <AL/alc.h>

#define  SAMPLE_RATE  16000
#define  MAX_VALUE 32760

int lxaudio::open = 0;

void
lxaudio::Init(int bcount_)
{
 if (!lxaudio::open)
  {
   bcount = bcount_;
   if(bcount > MAXBUFF)bcount=MAXBUFF;
   const char* defname = alcGetString (NULL, ALC_DEFAULT_DEVICE_SPECIFIER);
   ALCdevice* dev = alcOpenDevice (defname);
   ALCcontext* ctx = alcCreateContext (dev, NULL);
   alcMakeContextCurrent (ctx);
  }
 lxaudio::open++;

 alGenBuffers (bcount, buf);
 alGenSources (1, &src);
 int_buf_size = 0;
}

void
lxaudio::End(void)
{
 alDeleteSources (1, &src);
 alDeleteBuffers (bcount, buf);

 lxaudio::open--;

 if (!lxaudio::open)
  {
   ALCcontext* ctx = alcGetCurrentContext ();
   ALCdevice* dev = alcGetContextsDevice (ctx);
   alcMakeContextCurrent (0);
   alcDestroyContext (ctx);
   alcCloseDevice (dev);
  }
}

unsigned int
lxaudio::GetSampleRate(void)
{
 return SAMPLE_RATE;
}

unsigned int
lxaudio::GetMax(void)
{
 return MAX_VALUE;
}

void
lxaudio::BeepStart(float freq, float volume)
{
 ALint queue;
 alGetSourcei (src, AL_BUFFERS_QUEUED, &queue);

 if (queue == 0)
  {
   short* samples;

   size_t buf_size = SAMPLE_RATE;

   samples = new short[buf_size];

   if (samples == 0)
    {
     printf ("It seems there is no more heap memory. Sorry we cannot make a beep!");
    }
   for (unsigned i = 0; i < buf_size; i++)
    samples[i] = volume * 32760 * sin (2 * M_PI * i * freq / SAMPLE_RATE);

   alBufferData (buf[0], AL_FORMAT_MONO16, samples, buf_size, SAMPLE_RATE);
   delete[] samples;

   alSourceQueueBuffers (src, 1, buf);

   alSourcei (src, AL_LOOPING, AL_TRUE);

   alSourcePlay (src);
  }
}

void
lxaudio::BeepStop(void)
{
 ALint proc;
 ALint queue;
 ALuint buffer;

 alGetSourcei (src, AL_BUFFERS_QUEUED, &queue);
 if (queue)
  {
   alSourcei (src, AL_LOOPING, AL_FALSE);

   alSourceStop (src);

   alGetSourcei (src, AL_BUFFERS_PROCESSED, &proc);
   while (proc == 0)
    {
     alGetSourcei (src, AL_BUFFERS_PROCESSED, &proc);
    }

   alSourceUnqueueBuffers (src, 1, &buffer);
  }
}

int
lxaudio::SoundProcess(void)
{

 ALint status;
 ALint proc;
 ALuint buffer[MAXBUFF];

 if (!int_buf_size)return 1;


 alGetSourcei (src, AL_BUFFERS_PROCESSED, &proc);
 
 if (proc == 0) return 0;

 alSourceUnqueueBuffers (src, proc, buffer);
 for(int i=0; i < proc; i++)
  {
   alBufferData (buffer[i], AL_FORMAT_MONO16, int_samples, int_buf_size, SAMPLE_RATE);
  }
 alSourceQueueBuffers (src, proc, buffer);

 alGetSourcei (src, AL_SOURCE_STATE, &status);

 if (status != AL_PLAYING)
  {
   alSourcePlay (src);
  }

 int_buf_size = 0;
 return 1;
}

int
lxaudio::IsPlaying(void)
{
 ALint status;
 alGetSourcei (src, AL_SOURCE_STATE, &status);
 if (status == AL_PLAYING)
  return 1;
 else
  return 0;
}

int
lxaudio::SoundPlay(short * samples, size_t buf_size, int ibfill)
{
 ALint queue;
 alGetSourcei (src, AL_BUFFERS_QUEUED, &queue);
 if (queue < bcount)
  {
   int free = bcount-queue;

   if(ibfill > free)ibfill=free;
   for(int i=queue; i < bcount;i++)
     alBufferData (buf[i], AL_FORMAT_MONO16, samples, buf_size, SAMPLE_RATE);
   alSourceQueueBuffers (src, ibfill, &buf[queue]);
   alSourcePlay (src);
   int_buf_size = 0;
   return 1;
  }
 else
  {
   if (!int_buf_size){
     int_samples = samples;
     int_buf_size = buf_size;
   }
   return SoundProcess ();
  }
}
