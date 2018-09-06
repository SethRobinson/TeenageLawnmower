//Direct Sound functions, by Seth A. Robinson



#include "ssound.h"


LPDIRECTSOUND		lpds;           // directsound interface pointer
DSBUFFERDESC		dsbd;           // directsound description
DSCAPS				dscaps;         // directsound caps
HRESULT				dsresult;       // general directsound result
DSBCAPS             dsbcaps;        // directsound buffer caps

LPDIRECTSOUNDBUFFER	lpdsbprimary,    // you won't need this normally
                    lpdsbsecondary;  // the sound buffers

WAVEFORMATEX        pcmwf;          // generic waveformat structure




pcm_sound			sound_fx[MAX_SOUNDS]; 

bool init_direct_sound(HWND hw)
{
  if (DirectSoundCreate(NULL, &lpds, NULL) != DS_OK)
  {
   Msg("Error initting direct sound.");
   return false;

  }

  if (lpds->SetCooperativeLevel(hw, DSSCL_NORMAL) != DS_OK)
  {
   Msg("Error setting dsound cooperative level");
   return false;
  }

// clear array out
memset(sound_fx,0,sizeof(pcm_sound)*MAX_SOUNDS);
	
// initialize the sound fx array
for (int index=0; index<MAX_SOUNDS; index++)
	{
	// test if this sound has been loaded
	if (sound_fx[index].dsbuffer)
		{
		// stop the sound
		sound_fx[index].dsbuffer->Stop();

		// release the buffer
		sound_fx[index].dsbuffer->Release();
	
		} // end if

	// clear the record out
	memset(&sound_fx[index],0,sizeof(pcm_sound));

	// now set up the fields
	sound_fx[index].state = SOUND_NULL;
	sound_fx[index].id    = index;

	} // end for index

/*
// load a wav file in
if ((sound_id = DSound_Load_WAV("FLIGHT.WAV"))!=-1)
   {
   // start the voc playing in looping mode
   sound_fx[sound_id].dsbuffer->Play(0,0,DSBPLAY_LOOPING);
   } // end if
  */
// return success
return(1);

}

void dsound_play(int sound_id)
{
 sound_fx[sound_id].dsbuffer->Play(0,0,NULL);

}

void kill_direct_sound (void)
{
  
	if (lpds != NULL)
	lpds->Release();

}


int DSound_Load_WAV(char *filename, int control_flags)
{
// this function loads a .wav file, sets up the directsound 
// buffer and loads the data into memory, the function returns 
// the id number of the sound


HMMIO 			hwav;    // handle to wave file
MMCKINFO		parent,  // parent chunk
                child;   // child chunk
WAVEFORMATEX    wfmtx;   // wave format structure

int	sound_id = -1,       // id of sound to be loaded
	index;               // looping variable

UCHAR *snd_buffer,       // temporary sound buffer to hold voc data
      *audio_ptr_1=NULL, // data ptr to first write buffer 
	  *audio_ptr_2=NULL; // data ptr to second write buffer

DWORD audio_length_1=0,  // length of first write buffer
	  audio_length_2=0;  // length of second write buffer
			
// step one: are there any open id's ?
for (index=0; index < MAX_SOUNDS; index++)
	{	
    // make sure this sound is unused
	if (sound_fx[index].state==SOUND_NULL)
	   {
	   sound_id = index;
	   break;
	   } // end if

	} // end for index

// did we get a free id?
if (sound_id==-1)
	return(-1);

// set up chunk info structure
parent.ckid 	    = (FOURCC)0;
parent.cksize 	    = 0;
parent.fccType	    = (FOURCC)0;
parent.dwDataOffset = 0;
parent.dwFlags		= 0;

// copy data
child = parent;

// open the WAV file
if ((hwav = mmioOpen(filename, NULL, MMIO_READ | MMIO_ALLOCBUF))==NULL)
    return(-1);

// descend into the RIFF 
parent.fccType = mmioFOURCC('W', 'A', 'V', 'E');

if (mmioDescend(hwav, &parent, NULL, MMIO_FINDRIFF))
    {
    // close the file
    mmioClose(hwav, 0);

    // return error, no wave section
    return(-1); 	
    } // end if

// descend to the WAVEfmt 
child.ckid = mmioFOURCC('f', 'm', 't', ' ');

if (mmioDescend(hwav, &child, &parent, 0))
    {
    // close the file
    mmioClose(hwav, 0);

    // return error, no format section
    return(-1); 	
    } // end if

// now read the wave format information from file
if (mmioRead(hwav, (char *)&wfmtx, sizeof(wfmtx)) != sizeof(wfmtx))
    {
    // close file
    mmioClose(hwav, 0);

    // return error, no wave format data
    return(-1);
    } // end if

// make sure that the data format is PCM
if (wfmtx.wFormatTag != WAVE_FORMAT_PCM)
    {
    // close the file
    mmioClose(hwav, 0);

    // return error, not the right data format
    return(-1); 
    } // end if

// now ascend up one level, so we can access data chunk
if (mmioAscend(hwav, &child, 0))
   {
   // close file
   mmioClose(hwav, 0);

   // return error, couldn't ascend
   return(-1); 	
   } // end if

// descend to the data chunk 
child.ckid = mmioFOURCC('d', 'a', 't', 'a');

if (mmioDescend(hwav, &child, &parent, MMIO_FINDCHUNK))
    {
    // close file
    mmioClose(hwav, 0);

    // return error, no data
    return(-1); 	
    } // end if

// finally!!!! now all we have to do is read the data in and
// set up the directsound buffer

// allocate the memory to load sound data
snd_buffer = (UCHAR *)malloc(child.cksize);

// read the wave data 
mmioRead(hwav, (char *)snd_buffer, child.cksize);

// close the file
mmioClose(hwav, 0);

// set rate and size in data structure
sound_fx[sound_id].rate  = wfmtx.nSamplesPerSec;
sound_fx[sound_id].size  = child.cksize;
sound_fx[sound_id].state = SOUND_LOADED;

// set up the format data structure
memset(&pcmwf, 0, sizeof(WAVEFORMATEX));

pcmwf.wFormatTag	  = WAVE_FORMAT_PCM;  // pulse code modulation
pcmwf.nChannels		  = wfmtx.nChannels;                // mono 
pcmwf.nSamplesPerSec  = wfmtx.nSamplesPerSec;            // always this rate
pcmwf.nBlockAlign	  = wfmtx.nBlockAlign;                
pcmwf.nAvgBytesPerSec = pcmwf.nSamplesPerSec * pcmwf.nBlockAlign;
pcmwf.wBitsPerSample  = wfmtx.wBitsPerSample;
pcmwf.cbSize		  = wfmtx.cbSize;

// prepare to create sounds buffer
dsbd.dwSize			= sizeof(DSBUFFERDESC);
dsbd.dwFlags		= control_flags | DSBCAPS_STATIC | DSBCAPS_LOCSOFTWARE;
dsbd.dwBufferBytes	= child.cksize;
dsbd.lpwfxFormat	= &pcmwf;

// create the sound buffer
if (FAILED(lpds->CreateSoundBuffer(&dsbd,&sound_fx[sound_id].dsbuffer,NULL)))
   {
   // release memory
   free(snd_buffer);

   // return error
   return(-1);
   } // end if

// copy data into sound buffer
if (FAILED(sound_fx[sound_id].dsbuffer->Lock(0,					 
								      child.cksize,			
								      (void **) &audio_ptr_1, 
								      &audio_length_1,
								      (void **)&audio_ptr_2, 
								      &audio_length_2,
								      DSBLOCK_FROMWRITECURSOR)))
								 return(0);

// copy first section of circular buffer
memcpy(audio_ptr_1, snd_buffer, audio_length_1);

// copy last section of circular buffer
memcpy(audio_ptr_2, (snd_buffer+audio_length_1),audio_length_2);

// unlock the buffer
if (FAILED(sound_fx[sound_id].dsbuffer->Unlock(audio_ptr_1, 
									    audio_length_1, 
									    audio_ptr_2, 
									    audio_length_2)))
 							     return(0);

// release the temp buffer
free(snd_buffer);

// return id
return(sound_id);

} // end DSound_Load_WAV
