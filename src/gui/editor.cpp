/*
Oxe FM Synth: a software synthesizer
Copyright (C) 2004-2015  Daniel Moura <oxe@oxesoft.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <strsafe.h>
#include <math.h>
#include "editor.h"
#include "mapper.h"
#include "resource.h"

#define TIMER_RESOLUTION_MS 20
#define BMP_PATH            "skin\\"

CEditor::CEditor(HINSTANCE hInstance, CSynthesizer *synthesizer)
{
    char dll_path[MAX_PATH];
    char tmp_path[MAX_PATH];
    int ctlcount = 0;
    GetDllPath(dll_path, MAX_PATH);
    StringCchCopyA(tmp_path, MAX_PATH, dll_path);
    StringCchCatA (tmp_path, MAX_PATH, BMP_PATH"chars.bmp");
    bmpchars   = (HBITMAP)LoadImageA(NULL, tmp_path, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    StringCchCopyA(tmp_path, MAX_PATH, dll_path);
    StringCchCatA (tmp_path, MAX_PATH, BMP_PATH"knob.bmp");
    bmpknob    = (HBITMAP)LoadImageA(NULL, tmp_path, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    StringCchCopyA(tmp_path, MAX_PATH, dll_path);
    StringCchCatA (tmp_path, MAX_PATH, BMP_PATH"knob2.bmp");
    bmpknob2   = (HBITMAP)LoadImageA(NULL, tmp_path, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    StringCchCopyA(tmp_path, MAX_PATH, dll_path);
    StringCchCatA (tmp_path, MAX_PATH, BMP_PATH"knob3.bmp");
    bmpknob3   = (HBITMAP)LoadImageA(NULL, tmp_path, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    StringCchCopyA(tmp_path, MAX_PATH, dll_path);
    StringCchCatA (tmp_path, MAX_PATH, BMP_PATH"key.bmp");
    bmpkey     = (HBITMAP)LoadImageA(NULL, tmp_path, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    StringCchCopyA(tmp_path, MAX_PATH, dll_path);
    StringCchCatA (tmp_path, MAX_PATH, BMP_PATH"bg.bmp");
    bmpbg      = (HBITMAP)LoadImageA(NULL, tmp_path, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    StringCchCopyA(tmp_path, MAX_PATH, dll_path);
    StringCchCatA (tmp_path, MAX_PATH, BMP_PATH"buttons.bmp ");
    bmpbuttons = (HBITMAP)LoadImageA(NULL, tmp_path, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    StringCchCopyA(tmp_path, MAX_PATH, dll_path);
    StringCchCatA (tmp_path, MAX_PATH, BMP_PATH"ops.bmp");
    bmpops     = (HBITMAP)LoadImageA(NULL, tmp_path, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    if (!bmpchars        ) bmpchars   = LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_CARACTERES));
    if (!bmpknob         ) bmpknob    = LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_KNOB));
    if (!bmpknob2        ) bmpknob2   = LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_KNOB2));
    if (!bmpknob3        ) bmpknob3   = LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_KNOB3));
    if (!bmpkey          ) bmpkey     = LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_CHAVE));
    if (!bmpbg           ) bmpbg      = LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_FUNDO));
    if (!bmpbuttons      ) bmpbuttons = LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_BUTTONS));
    if (!bmpops          ) bmpops     = LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_OPS));
    
    this->synthesizer = synthesizer;
    this->hwnd        = 0;
    channel           = 0;
    
    // Configuração de posicionamento dos controles da interface
    int aX   = 0;    // auxiliar
    int aY   = 0;    // auxiliar
    
    int sX   = 32;   // separação horizontal entre os controles dos operadores
    int sX2  = sX/2; // metade de sX
    int sY   = 35;   // separação vertical entre os controles dos operadores

    int oX   = 202;  // separação horizontal entre os operadores
    int oY   = 94;   // separação vertical entre os operadores
    
    int fX   = 20;   // separação horizontal entre os controles da matriz
    int fY   = 20;   // separação vertical entre os controles da matriz

    int tX   = 44;   // separação horizontal entre os botões
    int tY   = 16;   // separação vertical entre os botões

    int cX   = 17;   // coordenada horizontal de OPA
    int cY   = 80;   // coordenada vertical de OPA
    
    int mX   = 419;  // coordenada horizontal da matriz
    int mY   = 77;   // coordenada vertical da matriz

    int lX   = 421;  // coordenada horizontal da seção LFO
    int lY   = 338;  // coordenada vertical da seção LFO

    int pX   = 421;  // coordenada horizontal da seção Pitch
    int pY   = 397;  // coordenada vertical da seção Pitch

    int zX   = 527;  // coordenada horizontal da seção Effects
    int zY   = 397;  // coordenada vertical da seção Effects

    int dX   = 591;  // coordenada horizontal da seção Modulation
    int dY   = 338;  // coordenada vertical da seção Modulation

    int gX   = 327;  // coordenada horizontal da seção Canais
    int gY   = 25;   // coordenada vertical da seção Canais

    int rX   = 559;  // coordenada horizontal da seção Reverb
    int rY   = 23;   // coordenada vertical da seção Reverb

    int eX   = 421;  // coordenada horizontal da seção Delay
    int eY   = 23;   // coordenada vertical da seção Delay

    int bX   = 147;  // coordenada horizontal da seção Botões
    int bY   = 15;   // coordenada vertical da seção Botões

    int hX   = 603;  // coordenada horizontal da seção HQ
    int hY   = 401;  // coordenada vertical da seção HQ

    // Display
    lcd             = new CLcd     (bmpchars, 25, 23);
    
    // OPA
    aX = cX;
    aY = cY;
    ctl[ctlcount++] = new CKey     (bmpkey,  0,10,10,"OPA On/Off",      synthesizer, channel,                   OPAON, aX         , aY      );
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "OPA Waveform",    synthesizer, channel, VL_WAVEFORM,      OPAWF, aX+sX2     , aY      );
    ctl[ctlcount++] = new CKey     (bmpkey,  0,10,10,"OPA Keyb Track",  synthesizer, channel,                   OPAKT, aX+sX+sX2  , aY      );
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "OPA Coarse Tune", synthesizer, channel, VL_COARSE_TUNE,   OPACT, aX+sX*2    , aY      );
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "OPA Fine Tune",   synthesizer, channel, VL_FINE_TUNE,     OPAFT, aX+sX*3    , aY      );
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "OPA VlSensivity", synthesizer, channel, VL_ZERO_TO_ONE,   OPAVS, aX+sX*4    , aY      );
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "OPA KeybScaling", synthesizer, channel, VL_MINUS1_2_PLUS1,OPAKS, aX+sX*5    , aY      );
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "OPA Delay Time",  synthesizer, channel, VL_PORTAMENTO,    OPADL, aX         , aY+sY   );
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "OPA Attack Time", synthesizer, channel, VL_TEMPO,         OPAAT, aX+sX      , aY+sY   );
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "OPA Decay Time",  synthesizer, channel, VL_TEMPO,         OPADE, aX+sX*2    , aY+sY   );
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "OPA Sustain Lvl", synthesizer, channel, VL_ZERO_TO_ONE,   OPASU, aX+sX*3    , aY+sY   );
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "OPA SustainTime", synthesizer, channel, VL_PORTAMENTO,    OPAST, aX+sX*4    , aY+sY   );
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "OPA ReleaseTime", synthesizer, channel, VL_TEMPO,         OPARE, aX+sX*5    , aY+sY   );
    
    // OPB
    aX = cX + oX;
    aY = cY;
    ctl[ctlcount++] = new CKey     (bmpkey,  0,10,10,"OPB On/Off",      synthesizer, channel,                   OPBON, aX         , aY      );
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "OPB Waveform",    synthesizer, channel, VL_WAVEFORM,      OPBWF, aX+sX2     , aY      );
    ctl[ctlcount++] = new CKey     (bmpkey,  0,10,10,"OPB Keyb Track",  synthesizer, channel,                   OPBKT, aX+sX+sX2  , aY      );
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "OPB Coarse Tune", synthesizer, channel, VL_COARSE_TUNE,   OPBCT, aX+sX*2    , aY      );
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "OPB Fine Tune",   synthesizer, channel, VL_FINE_TUNE,     OPBFT, aX+sX*3    , aY      );
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "OPB VlSensivity", synthesizer, channel, VL_ZERO_TO_ONE,   OPBVS, aX+sX*4    , aY      );
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "OPB KeybScaling", synthesizer, channel, VL_MINUS1_2_PLUS1,OPBKS, aX+sX*5    , aY      );
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "OPB Delay Time",  synthesizer, channel, VL_PORTAMENTO,    OPBDL, aX         , aY+sY   );
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "OPB Attack Time", synthesizer, channel, VL_TEMPO,         OPBAT, aX+sX      , aY+sY   );
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "OPB Decay Time",  synthesizer, channel, VL_TEMPO,         OPBDE, aX+sX*2    , aY+sY   );
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "OPB Sustain Lvl", synthesizer, channel, VL_ZERO_TO_ONE,   OPBSU, aX+sX*3    , aY+sY   );
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "OPB SustainTime", synthesizer, channel, VL_PORTAMENTO,    OPBST, aX+sX*4    , aY+sY   );
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "OPB ReleaseTime", synthesizer, channel, VL_TEMPO,         OPBRE, aX+sX*5    , aY+sY   );
    
    // OPC
    aX = cX;
    aY = cY + oY;
    ctl[ctlcount++] = new CKey     (bmpkey,  0,10,10,"OPC On/Off",      synthesizer, channel,                   OPCON, aX         , aY      );
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "OPC Waveform",    synthesizer, channel, VL_WAVEFORM,      OPCWF, aX+sX2     , aY      );
    ctl[ctlcount++] = new CKey     (bmpkey,  0,10,10,"OPC Keyb Track",  synthesizer, channel,                   OPCKT, aX+sX+sX2  , aY      );
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "OPC Coarse Tune", synthesizer, channel, VL_COARSE_TUNE,   OPCCT, aX+sX*2    , aY      );
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "OPC Fine Tune",   synthesizer, channel, VL_FINE_TUNE,     OPCFT, aX+sX*3    , aY      );
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "OPC VlSensivity", synthesizer, channel, VL_ZERO_TO_ONE,   OPCVS, aX+sX*4    , aY      );
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "OPC KeybScaling", synthesizer, channel, VL_MINUS1_2_PLUS1,OPCKS, aX+sX*5    , aY      );
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "OPC Delay Time",  synthesizer, channel, VL_PORTAMENTO,    OPCDL, aX         , aY+sY   );
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "OPC Attack Time", synthesizer, channel, VL_TEMPO,         OPCAT, aX+sX      , aY+sY   );
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "OPC Decay Time",  synthesizer, channel, VL_TEMPO,         OPCDE, aX+sX*2    , aY+sY   );
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "OPC Sustain Lvl", synthesizer, channel, VL_ZERO_TO_ONE,   OPCSU, aX+sX*3    , aY+sY   );
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "OPC SustainTime", synthesizer, channel, VL_PORTAMENTO,    OPCST, aX+sX*4    , aY+sY   );
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "OPC ReleaseTime", synthesizer, channel, VL_TEMPO,         OPCRE, aX+sX*5    , aY+sY   );
    
    // OPD
    aX = cX + oX;
    aY = cY + oY;
    ctl[ctlcount++] = new CKey     (bmpkey,  0,10,10,"OPD On/Off",      synthesizer, channel,                   OPDON, aX         , aY      );
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "OPD Waveform",    synthesizer, channel, VL_WAVEFORM,      OPDWF, aX+sX2     , aY      );
    ctl[ctlcount++] = new CKey     (bmpkey,  0,10,10,"OPD Keyb Track",  synthesizer, channel,                   OPDKT, aX+sX+sX2  , aY      );
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "OPD Coarse Tune", synthesizer, channel, VL_COARSE_TUNE,   OPDCT, aX+sX*2    , aY      );
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "OPD Fine Tune",   synthesizer, channel, VL_FINE_TUNE,     OPDFT, aX+sX*3    , aY      );
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "OPD VlSensivity", synthesizer, channel, VL_ZERO_TO_ONE,   OPDVS, aX+sX*4    , aY      );
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "OPD KeybScaling", synthesizer, channel, VL_MINUS1_2_PLUS1,OPDKS, aX+sX*5    , aY      );
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "OPD Delay Time",  synthesizer, channel, VL_PORTAMENTO,    OPDDL, aX         , aY+sY   );
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "OPD Attack Time", synthesizer, channel, VL_TEMPO,         OPDAT, aX+sX      , aY+sY   );
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "OPD Decay Time",  synthesizer, channel, VL_TEMPO,         OPDDE, aX+sX*2    , aY+sY   );
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "OPD Sustain Lvl", synthesizer, channel, VL_ZERO_TO_ONE,   OPDSU, aX+sX*3    , aY+sY   );
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "OPD SustainTime", synthesizer, channel, VL_PORTAMENTO,    OPDST, aX+sX*4    , aY+sY   );
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "OPD ReleaseTime", synthesizer, channel, VL_TEMPO,         OPDRE, aX+sX*5    , aY+sY   );
    
    // OPE
    aX = cX;
    aY = cY + oY * 2;
    ctl[ctlcount++] = new CKey     (bmpkey,  0,10,10,"OPE On/Off",      synthesizer, channel,                   OPEON, aX         , aY      );
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "OPE Waveform",    synthesizer, channel, VL_WAVEFORM,      OPEWF, aX+sX2     , aY      );
    ctl[ctlcount++] = new CKey     (bmpkey,  0,10,10,"OPE Keyb Track",  synthesizer, channel,                   OPEKT, aX+sX+sX2  , aY      );
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "OPE Coarse Tune", synthesizer, channel, VL_COARSE_TUNE,   OPECT, aX+sX*2    , aY      );
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "OPE Fine Tune",   synthesizer, channel, VL_FINE_TUNE,     OPEFT, aX+sX*3    , aY      );
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "OPE VlSensivity", synthesizer, channel, VL_ZERO_TO_ONE,   OPEVS, aX+sX*4    , aY      );
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "OPE KeybScaling", synthesizer, channel, VL_MINUS1_2_PLUS1,OPEKS, aX+sX*5    , aY      );
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "OPE Delay Time",  synthesizer, channel, VL_PORTAMENTO,    OPEDL, aX         , aY+sY   );
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "OPE Attack Time", synthesizer, channel, VL_TEMPO,         OPEAT, aX+sX      , aY+sY   );
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "OPE Decay Time",  synthesizer, channel, VL_TEMPO,         OPEDE, aX+sX*2    , aY+sY   );
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "OPE Sustain Lvl", synthesizer, channel, VL_ZERO_TO_ONE,   OPESU, aX+sX*3    , aY+sY   );
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "OPE SustainTime", synthesizer, channel, VL_PORTAMENTO,    OPEST, aX+sX*4    , aY+sY   );
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "OPE ReleaseTime", synthesizer, channel, VL_TEMPO,         OPERE, aX+sX*5    , aY+sY   );
    
    // OPF
    aX = cX + oX;
    aY = cY + oY * 2;
    ctl[ctlcount++] = new CKey     (bmpkey,  0,10,10,"OPF On/Off",      synthesizer, channel,                   OPFON, aX         , aY      );
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "OPF Waveform",    synthesizer, channel, VL_WAVEFORM,      OPFWF, aX+sX2     , aY      );
    ctl[ctlcount++] = new CKey     (bmpkey,  0,10,10,"OPF Keyb Track",  synthesizer, channel,                   OPFKT, aX+sX+sX2  , aY      );
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "OPF Coarse Tune", synthesizer, channel, VL_COARSE_TUNE,   OPFCT, aX+sX*2    , aY      );
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "OPF Fine Tune",   synthesizer, channel, VL_FINE_TUNE,     OPFFT, aX+sX*3    , aY      );
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "OPF VlSensivity", synthesizer, channel, VL_ZERO_TO_ONE,   OPFVS, aX+sX*4    , aY      );
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "OPF KeybScaling", synthesizer, channel, VL_MINUS1_2_PLUS1,OPFKS, aX+sX*5    , aY      );
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "OPF Delay Time",  synthesizer, channel, VL_PORTAMENTO,    OPFDL, aX         , aY+sY   );
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "OPF Attack Time", synthesizer, channel, VL_TEMPO,         OPFAT, aX+sX      , aY+sY   );
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "OPF Decay Time",  synthesizer, channel, VL_TEMPO,         OPFDE, aX+sX*2    , aY+sY   );
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "OPF Sustain Lvl", synthesizer, channel, VL_ZERO_TO_ONE,   OPFSU, aX+sX*3    , aY+sY   );
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "OPF SustainTime", synthesizer, channel, VL_PORTAMENTO,    OPFST, aX+sX*4    , aY+sY   );
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "OPF ReleaseTime", synthesizer, channel, VL_TEMPO,         OPFRE, aX+sX*5    , aY+sY   );
    
    // OPX
    aX = cX;
    aY = cY + oY * 3;
    ctl[ctlcount++] = new CKey     (bmpkey,  0,10,10,"OPX On/Off",      synthesizer, channel,                   OPXON, aX         , aY      );
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "OPX Cuttof",      synthesizer, channel, VL_FILTER_CUTOFF, OPXCU, aX+sX2     , aY      );
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "OPX Resonance",   synthesizer, channel, VL_ZERO_TO_ONE,   OPXRS, aX+sX+sX2  , aY      );
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "OPX Amount",      synthesizer, channel, VL_ZERO_TO_ONE,   OPXAM, aX+sX*2+sX2, aY      );
    ctl[ctlcount++] = new CKey     (bmpkey,  0,10,10,"OPX Bypass",      synthesizer, channel,                   OPXBP, aX+sX*3+sX2, aY      );
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "OPX VlSensivity", synthesizer, channel, VL_ZERO_TO_ONE,   OPXVS, aX+sX*4    , aY      );
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "OPX KeybScaling", synthesizer, channel, VL_MINUS1_2_PLUS1,OPXKS, aX+sX*5    , aY      );
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "OPX Delay Time",  synthesizer, channel, VL_PORTAMENTO,    OPXDL, aX         , aY+sY   );
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "OPX Attack Time", synthesizer, channel, VL_TEMPO,         OPXAT, aX+sX      , aY+sY   );
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "OPX Decay Time",  synthesizer, channel, VL_TEMPO,         OPXDE, aX+sX*2    , aY+sY   );
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "OPX Sustain Lvl", synthesizer, channel, VL_ZERO_TO_ONE,   OPXSU, aX+sX*3    , aY+sY   );
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "OPX SustainTime", synthesizer, channel, VL_PORTAMENTO,    OPXST, aX+sX*4    , aY+sY   );
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "OPX ReleaseTime", synthesizer, channel, VL_TEMPO,         OPXRE, aX+sX*5    , aY+sY   );
    
    // OPZ
    aX = cX + oX;
    aY = cY + oY * 3;
    ctl[ctlcount++] = new CKey     (bmpkey,  0,10,10,"OPZ On/Off",      synthesizer, channel,                   OPZON, aX         , aY      );
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "OPZ Cuttof",      synthesizer, channel, VL_FILTER_CUTOFF, OPZCU, aX+sX2     , aY      );
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "OPZ Resonance",   synthesizer, channel, VL_ZERO_TO_ONE,   OPZRS, aX+sX+sX2  , aY      );
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "OPZ Type",        synthesizer, channel, VL_FILTER,        OPZTY, aX+sX*2+sX2, aY      );
    ctl[ctlcount++] = new CKey     (bmpkey,  0,10,10,"OPZ Keyb Track",  synthesizer, channel,                   OPZKT, aX+sX*3+sX2, aY      );
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "OPX VlSensivity", synthesizer, channel, VL_ZERO_TO_ONE,   OPZVS, aX+sX*4    , aY      );
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "OPX KeybScaling", synthesizer, channel, VL_MINUS1_2_PLUS1,OPZKS, aX+sX*5    , aY      );
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "OPZ Delay Time",  synthesizer, channel, VL_PORTAMENTO,    OPZDL, aX         , aY+sY   );
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "OPZ Attack Time", synthesizer, channel, VL_TEMPO,         OPZAT, aX+sX      , aY+sY   );
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "OPZ Decay Time",  synthesizer, channel, VL_TEMPO,         OPZDE, aX+sX*2    , aY+sY   );
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "OPZ Sustain Lvl", synthesizer, channel, VL_ZERO_TO_ONE,   OPZSU, aX+sX*3    , aY+sY   );
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "OPZ SustainTime", synthesizer, channel, VL_PORTAMENTO,    OPZST, aX+sX*4    , aY+sY   );
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "OPZ ReleaseTime", synthesizer, channel, VL_TEMPO,         OPZRE, aX+sX*5    , aY+sY   );
    
    // Matriz
    aX = mX;
    aY = mY;
    ctl[ctlcount++] = new CKnob    (bmpknob2,  20,   "OPA Self Mod",    synthesizer, channel, VL_MOD,           MAA,     aX       , aY      );
    ctl[ctlcount++] = new CKey     (bmpops  ,0,20,20,"OPA On/Off",      synthesizer, channel,                   OPAON,   aX       , aY+fY   );
    ctl[ctlcount++] = new CKnob    (bmpknob2,  20,   "FM OPA->OPB",     synthesizer, channel, VL_MOD,           MAB,     aX       , aY+fY*2 );
    ctl[ctlcount++] = new CKnob    (bmpknob2,  20,   "FM OPA->OPC",     synthesizer, channel, VL_MOD,           MAC,     aX       , aY+fY*3 );
    ctl[ctlcount++] = new CKnob    (bmpknob2,  20,   "FM OPA->OPD",     synthesizer, channel, VL_MOD,           MAD,     aX       , aY+fY*4 );
    ctl[ctlcount++] = new CKnob    (bmpknob2,  20,   "FM OPA->OPE",     synthesizer, channel, VL_MOD,           MAE,     aX       , aY+fY*5 );
    ctl[ctlcount++] = new CKnob    (bmpknob2,  20,   "FM OPA->OPF",     synthesizer, channel, VL_MOD,           MAF,     aX       , aY+fY*6 );
    ctl[ctlcount++] = new CKnob    (bmpknob2,  20,   "FM OPA->OPX",     synthesizer, channel, VL_MOD,           MAX,     aX       , aY+fY*7 );
    ctl[ctlcount++] = new CKnob    (bmpknob2,  20,   "FM OPA->OPZ",     synthesizer, channel, VL_MOD,           MAZ,     aX       , aY+fY*8 );
    ctl[ctlcount++] = new CKnob    (bmpknob2,  20,   "OPA Output",      synthesizer, channel, VL_MOD,           MAO,     aX       , aY+fY*9 );
    ctl[ctlcount++] = new CKnob    (bmpknob3,  20,   "OPA Pan",         synthesizer, channel, VL_PAN,           MAP,     aX       , aY+fY*10);

    ctl[ctlcount++] = new CKnob    (bmpknob2,  20,   "OPB Self Mod",    synthesizer, channel, VL_MOD,           MBB,     aX+fX    , aY+fY   );
    ctl[ctlcount++] = new CKey     (bmpops  ,1,20,20,"OPB On/Off",      synthesizer, channel,                   OPBON,   aX+fX    , aY+fY*2 );
    ctl[ctlcount++] = new CKnob    (bmpknob2,  20,   "FM OPB->OPC",     synthesizer, channel, VL_MOD,           MBC,     aX+fX    , aY+fY*3 );
    ctl[ctlcount++] = new CKnob    (bmpknob2,  20,   "FM OPB->OPD",     synthesizer, channel, VL_MOD,           MBD,     aX+fX    , aY+fY*4 );
    ctl[ctlcount++] = new CKnob    (bmpknob2,  20,   "FM OPB->OPE",     synthesizer, channel, VL_MOD,           MBE,     aX+fX    , aY+fY*5 );
    ctl[ctlcount++] = new CKnob    (bmpknob2,  20,   "FM OPB->OPF",     synthesizer, channel, VL_MOD,           MBF,     aX+fX    , aY+fY*6 );
    ctl[ctlcount++] = new CKnob    (bmpknob2,  20,   "FM OPB->OPX",     synthesizer, channel, VL_MOD,           MBX,     aX+fX    , aY+fY*7 );
    ctl[ctlcount++] = new CKnob    (bmpknob2,  20,   "FM OPB->OPZ",     synthesizer, channel, VL_MOD,           MBZ,     aX+fX    , aY+fY*8 );
    ctl[ctlcount++] = new CKnob    (bmpknob2,  20,   "OPB Output",      synthesizer, channel, VL_MOD,           MBO,     aX+fX    , aY+fY*9 );
    ctl[ctlcount++] = new CKnob    (bmpknob3,  20,   "OPB Pan",         synthesizer, channel, VL_PAN,           MBP,     aX+fX    , aY+fY*10);

    ctl[ctlcount++] = new CKnob    (bmpknob2,  20,   "OPC Self Mod",    synthesizer, channel, VL_MOD,           MCC,     aX+fX*2  , aY+fY*2 );
    ctl[ctlcount++] = new CKey     (bmpops  ,2,20,20,"OPC On/Off",      synthesizer, channel,                   OPCON,   aX+fX*2  , aY+fY*3 );
    ctl[ctlcount++] = new CKnob    (bmpknob2,  20,   "FM OPC->OPD",     synthesizer, channel, VL_MOD,           MCD,     aX+fX*2  , aY+fY*4 );
    ctl[ctlcount++] = new CKnob    (bmpknob2,  20,   "FM OPC->OPE",     synthesizer, channel, VL_MOD,           MCE,     aX+fX*2  , aY+fY*5 );
    ctl[ctlcount++] = new CKnob    (bmpknob2,  20,   "FM OPC->OPF",     synthesizer, channel, VL_MOD,           MCF,     aX+fX*2  , aY+fY*6 );
    ctl[ctlcount++] = new CKnob    (bmpknob2,  20,   "FM OPC->OPX",     synthesizer, channel, VL_MOD,           MCX,     aX+fX*2  , aY+fY*7 );
    ctl[ctlcount++] = new CKnob    (bmpknob2,  20,   "FM OPC->OPZ",     synthesizer, channel, VL_MOD,           MCZ,     aX+fX*2  , aY+fY*8 );
    ctl[ctlcount++] = new CKnob    (bmpknob2,  20,   "OPC Output",      synthesizer, channel, VL_MOD,           MCO,     aX+fX*2  , aY+fY*9 );
    ctl[ctlcount++] = new CKnob    (bmpknob3,  20,   "OPC Pan",         synthesizer, channel, VL_PAN,           MCP,     aX+fX*2  , aY+fY*10);

    ctl[ctlcount++] = new CKnob    (bmpknob2,  20,   "OPD Self Mod",    synthesizer, channel, VL_MOD,           MDD,     aX+fX*3  , aY+fY*3 );
    ctl[ctlcount++] = new CKey     (bmpops  ,3,20,20,"OPD On/Off",      synthesizer, channel,                   OPDON,   aX+fX*3  , aY+fY*4 );
    ctl[ctlcount++] = new CKnob    (bmpknob2,  20,   "FM OPD->OPE",     synthesizer, channel, VL_MOD,           MDE,     aX+fX*3  , aY+fY*5 );
    ctl[ctlcount++] = new CKnob    (bmpknob2,  20,   "FM OPD->OPF",     synthesizer, channel, VL_MOD,           MDF,     aX+fX*3  , aY+fY*6 );
    ctl[ctlcount++] = new CKnob    (bmpknob2,  20,   "FM OPD->OPX",     synthesizer, channel, VL_MOD,           MDX,     aX+fX*3  , aY+fY*7 );
    ctl[ctlcount++] = new CKnob    (bmpknob2,  20,   "FM OPD->OPZ",     synthesizer, channel, VL_MOD,           MDZ,     aX+fX*3  , aY+fY*8 );
    ctl[ctlcount++] = new CKnob    (bmpknob2,  20,   "OPD Output",      synthesizer, channel, VL_MOD,           MDO,     aX+fX*3  , aY+fY*9 );
    ctl[ctlcount++] = new CKnob    (bmpknob3,  20,   "OPD Pan",         synthesizer, channel, VL_PAN,           MDP,     aX+fX*3  , aY+fY*10);

    ctl[ctlcount++] = new CKnob    (bmpknob2,  20,   "OPE Self Mod",    synthesizer, channel, VL_MOD,           MEE,     aX+fX*4  , aY+fY*4 );
    ctl[ctlcount++] = new CKey     (bmpops  ,4,20,20,"OPE On/Off",      synthesizer, channel,                   OPEON,   aX+fX*4  , aY+fY*5 );
    ctl[ctlcount++] = new CKnob    (bmpknob2,  20,   "FM OPE->OPF",     synthesizer, channel, VL_MOD,           MEF,     aX+fX*4  , aY+fY*6 );
    ctl[ctlcount++] = new CKnob    (bmpknob2,  20,   "FM OPE->OPX",     synthesizer, channel, VL_MOD,           MEX,     aX+fX*4  , aY+fY*7 );
    ctl[ctlcount++] = new CKnob    (bmpknob2,  20,   "FM OPE->OPZ",     synthesizer, channel, VL_MOD,           MEZ,     aX+fX*4  , aY+fY*8 );
    ctl[ctlcount++] = new CKnob    (bmpknob2,  20,   "OPE Output",      synthesizer, channel, VL_MOD,           MEO,     aX+fX*4  , aY+fY*9 );
    ctl[ctlcount++] = new CKnob    (bmpknob3,  20,   "OPE Pan",         synthesizer, channel, VL_PAN,           MEP,     aX+fX*4  , aY+fY*10);

    ctl[ctlcount++] = new CKnob    (bmpknob2,  20,   "OPF Self Mod",    synthesizer, channel, VL_MOD,           MFF,     aX+fX*5  , aY+fY*5 );
    ctl[ctlcount++] = new CKey     (bmpops  ,5,20,20,"OPF On/Off",      synthesizer, channel,                   OPFON,   aX+fX*5  , aY+fY*6 );
    ctl[ctlcount++] = new CKnob    (bmpknob2,  20,   "FM OPF->OPX",     synthesizer, channel, VL_MOD,           MFX,     aX+fX*5  , aY+fY*7 );
    ctl[ctlcount++] = new CKnob    (bmpknob2,  20,   "FM OPF->OPZ",     synthesizer, channel, VL_MOD,           MFZ,     aX+fX*5  , aY+fY*8 );
    ctl[ctlcount++] = new CKnob    (bmpknob2,  20,   "OPF Output",      synthesizer, channel, VL_MOD,           MFO,     aX+fX*5  , aY+fY*9 );
    ctl[ctlcount++] = new CKnob    (bmpknob3,  20,   "OPF Pan",         synthesizer, channel, VL_PAN,           MFP,     aX+fX*5  , aY+fY*10);

    ctl[ctlcount++] = new CKey     (bmpops  ,6,20,20,"OPX On/Off",      synthesizer, channel,                   OPXON,   aX+fX*6  , aY+fY*7 );
    ctl[ctlcount++] = new CKnob    (bmpknob2,  20,   "FM OPX->OPZ",     synthesizer, channel, VL_MOD,           MXZ,     aX+fX*6  , aY+fY*8 );
    ctl[ctlcount++] = new CKnob    (bmpknob2,  20,   "OPX Output",      synthesizer, channel, VL_MOD,           MXO,     aX+fX*6  , aY+fY*9 );
    ctl[ctlcount++] = new CKnob    (bmpknob3,  20,   "OPX Pan",         synthesizer, channel, VL_PAN,           MXP,     aX+fX*6  , aY+fY*10);
    
    ctl[ctlcount++] = new CKey     (bmpops  ,7,20,20,"OPZ On/Off",      synthesizer, channel,                   OPZON,   aX+fX*7  , aY+fY*8 );
    ctl[ctlcount++] = new CKnob    (bmpknob2,  20,   "OPZ Output",      synthesizer, channel, VL_MOD,           MZO,     aX+fX*7  , aY+fY*9 );
    ctl[ctlcount++] = new CKnob    (bmpknob3,  20,   "OPZ Pan",         synthesizer, channel, VL_PAN,           MZP,     aX+fX*7  , aY+fY*10);

    // Pitch
    aX = pX;
    aY = pY;
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "Portamento",      synthesizer, channel, VL_PORTAMENTO,    PORTA, aX         , aY      );
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "Pitch Curve",     synthesizer, channel, VL_PITCH_CURVE,   PTCCU, aX+sX      , aY      );
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "PitchCurveTime",  synthesizer, channel, VL_PORTAMENTO,    PTCTI, aX+sX*2    , aY      );
    
    // LFO
    aX = lX;
    aY = lY;
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "LFO Waveform",    synthesizer, channel, VL_WAVEFORM,      LFOWF, aX         , aY      );
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "LFO Rate",        synthesizer, channel, VL_LFO_RATE,      LFORA, aX+sX      , aY      );
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "LFO Depth",       synthesizer, channel, VL_MOD,           LFODE, aX+sX*2    , aY      );
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "LFO Delay",       synthesizer, channel, VL_PORTAMENTO,    LFODL, aX+sX*3    , aY      );
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "LFO Destination", synthesizer, channel, VL_LFO_DEST,      LFODS, aX+sX*4    , aY      );

    // Modulation
    aX = dX;
    aY = dY;
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "Mod Destination", synthesizer, channel, VL_MOD_DEST,      MDLDS, aX         , aY      );
    
    // Canais
    aX = gX;
    aY = gY;
    ctl[ctlcount++] = new CChannels(bmpkey,                             synthesizer, channel,                          aX         , aY      );

    // Reverb
    aX = rX;
    aY = rY;
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "Reverb Time",     synthesizer, channel, VL_ZERO_TO_ONE,   REVTI, aX         , aY      );
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "Reverb Damp",     synthesizer, channel, VL_ZERO_TO_ONE,   REVDA, aX+sX      , aY      );
    
    // Delay
    aX = eX;
    aY = eY;
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "Delay Time",      synthesizer, channel, VL_ZERO_TO_ONE,   DLYTI, aX         , aY      );
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "Delay Feedback",  synthesizer, channel, VL_ZERO_TO_ONE,   DLYFE, aX+sX      , aY      );
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "Delay LFO Rate",  synthesizer, channel, VL_LFO_RATE,      DLYLF, aX+sX*2    , aY      );
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "Delay LFO Amt",   synthesizer, channel, VL_ZERO_TO_ONE,   DLYLA, aX+sX*3    , aY      );

    // Efeitos
    aX = zX;
    aY = zY;
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "Delay Level",     synthesizer, channel, VL_ZERO_TO_ONE,   DLYLV, aX         , aY      );
    ctl[ctlcount++] = new CKnob    (bmpknob,   25,   "Reverb Level",    synthesizer, channel, VL_ZERO_TO_ONE,   RVBLV, aX+sX      , aY      );

    // Botões
    aX = bX;
    aY = bY;
    ctl[ctlcount++] = new CButton  (bmpbuttons, 0,                      synthesizer, channel, BT_BANK,                 aX         , aY      );
    ctl[ctlcount++] = new CButton  (bmpbuttons, 1,                      synthesizer, channel, BT_PROGRAM,              aX         , aY+tY   );
    ctl[ctlcount++] = new CButton  (bmpbuttons, 2,                      synthesizer, channel, BT_MINUS_10,             aX+tX      , aY      );
    ctl[ctlcount++] = new CButton  (bmpbuttons, 3,                      synthesizer, channel, BT_MINUS_1,              aX+tX      , aY+tY   );
    ctl[ctlcount++] = new CButton  (bmpbuttons, 4,                      synthesizer, channel, BT_PLUS_1,               aX+tX*2    , aY+tY   );
    ctl[ctlcount++] = new CButton  (bmpbuttons, 5,                      synthesizer, channel, BT_PLUS_10,              aX+tX*2    , aY      );
    ctl[ctlcount++] = new CButton  (bmpbuttons, 6,                      synthesizer, channel, BT_NAME,                 aX+tX*3    , aY      );
    ctl[ctlcount++] = new CButton  (bmpbuttons, 7,                      synthesizer, channel, BT_STORE,                aX+tX*3    , aY+tY   );

    // HQ
    ctl[ctlcount++] = new CKey     (bmpkey,0,10,10,"High Quality",      synthesizer, channel,                   HQ   , hX         , hY      );

    if (ctlcount != GUI_CONTROLS)
    {
        MessageBox(NULL, "Error creating controls", TITLE_SMALL, MB_OK);
    }

    changingControl = FALSE;
    hdc    = NULL;
    hdcMem = NULL;
    bitmap = NULL;
}

CEditor::~CEditor()
{
    if (hwnd)
    {
        KillTimer(hwnd, 0);
    }
    for (int i=0;i<GUI_CONTROLS;i++)
    {
        delete ctl[i];
    }
    DeleteObject(bmpchars);
    DeleteObject(bmpknob);
    DeleteObject(bmpknob2);
    DeleteObject(bmpknob3);
    DeleteObject(bmpkey);
    DeleteObject(bmpbg);
    DeleteObject(bmpbuttons);
    DeleteObject(bmpops);
}

void CEditor::SetHandle(HWND hwnd)
{
    if (this->hwnd)
    {
        KillTimer(this->hwnd, 0);
        DeleteDC(hdcAux);
        DeleteDC(hdcMem);
        DeleteObject(bitmap);
        ReleaseDC(hwnd, hdc);
        hdc    = NULL;
        hdcMem = NULL;
        hdcAux = NULL;
    }
    if (hwnd)
    {
        // new offscreen buffer
        hdc = GetDC(hwnd); 
        hdcMem = CreateCompatibleDC(hdc);
        hdcAux = CreateCompatibleDC(hdc);
        bitmap = CreateCompatibleBitmap(hdc, GUI_WIDTH, GUI_HEIGHT);
        SelectObject(hdcMem, bitmap);
        SelectObject(hdcAux, bmpbg);
        BitBlt(hdcMem, 0, 0, GUI_WIDTH, GUI_HEIGHT, hdcAux, 0, 0, SRCCOPY);
    }
    lcd->SetHandlers(hwnd, hdcMem, hdcAux);
    for (int i = 0; i < GUI_CONTROLS; i++)
    {
        ctl[i]->SetHandlers(hwnd, hdcMem, hdcAux);
    }
    this->hwnd = hwnd;
    if (this->hwnd)
    {
        // show version
        char version_info[LCD_COLS + 1];
        StringCchCopyA(version_info, LCD_COLS, "                ");
        StringCchCopyA(version_info + (LCD_COLS / 2) - (strlen(VERSION_STR) / 2), LCD_COLS, VERSION_STR);
        SetTimer(this->hwnd,0,TIMER_RESOLUTION_MS,NULL);
        lcd->SetText(0, "  Oxe FM Synth  ");
        lcd->SetText(1, version_info);
    }
    changingControl = FALSE;
    synthesizer->SetEditorHn(this->hwnd);
}

void CEditor::ProgramChanged()
{
    char str[TEXT_SIZE];
    StringCchPrintfA(str, TEXT_SIZE, "Program %03i", synthesizer->GetNumProgr(channel));
    lcd->SetText(0,str);
    synthesizer->GetProgName(str, channel);
    lcd->SetText(1,str);
}

void CEditor::ProgramChangedWaiting()
{
    char str[TEXT_SIZE];
    StringCchPrintfA(str, TEXT_SIZE, "Store current");
    lcd->SetText(0,str);
    StringCchPrintfA(str, TEXT_SIZE, "conf in Prg%03i?",synthesizer->GetNumProgr(channel));
    lcd->SetText(1,str);
}

bool CEditor::OnChar(int cod)
{
    char str0[PG_NAME_SIZE + 1];
    char str1[PG_NAME_SIZE + 1];
    int  pos;
    if (!synthesizer->IsEditingName())
    {
        return false;
    }
    // edits the program name
    ZeroMemory(str1, sizeof(str1));
    synthesizer->GetProgName(str1, channel);
    for (pos = PG_NAME_SIZE - 1; pos>=0; pos--)
        if (str1[pos]>0)
            break;
    if (cod >= 32 && cod <= 126 && pos < PG_NAME_SIZE - 1)
    {
        str1[pos+1] = (char)cod;
        lcd->SetText(1,str1);
        synthesizer->SetProgName(str1, channel);
    }
    else if (cod == 8)
    {
        if (pos >= 0)
        {
            str1[pos] = 0;
            lcd->SetText(1, str1);
            synthesizer->SetProgName(str1, channel);
        }
    }
    StringCchPrintfA(str0, TEXT_SIZE, "Program %03i", synthesizer->GetNumProgr(channel));
    lcd->SetText(0, str0);
    return true;
}

void CEditor::OnLButtonDblClick(POINT point)
{
    for (int i=0;i<GUI_CONTROLS;i++)
    {
        if (ctl[i]->IsMouseOver(point))
        {
            int index = ctl[i]->GetIndex();
            if (index >= 0)
            {
                synthesizer->SetDefault(channel, index);
                if (channel == 0)
                {
                    PostMessage(hwnd, WM_SET_PARAMETER, index, GetPar(index) * MAXPARVALUE);
                }
            }
            break;
        }
    }
}

void CEditor::OnLButtonDown(POINT point)
{
    char str[TEXT_SIZE];
    char prevchannel = channel; // saves the current channel in case of the clicked control be CChannels
    for (int i=0;i<GUI_CONTROLS;i++)
    {
        if (ctl[i]->IsMouseOver(point))
        {
            ctl[i]->OnClick(point);
            synthesizer->SetEditingName(ctl[i]->GetType() == BT_NAME);
            if (synthesizer->GetStandBy(prevchannel))
            {
                synthesizer->SetStandBy(prevchannel, ctl[i]->GetType() != BT_NAME);
            }
            ctl[i]->GetName(str);
            lcd->SetText(0,str);
            CMapper::GetDisplayValue(this->synthesizer, this->channel, ctl[i]->GetIndex(), ctl[i]->GetType(), str);
            lcd->SetText(1,str);
            cID = i;
            if (ctl[i]->IsKnob() == true)
            {
                changingControl = TRUE;
                SetCapture(hwnd);
            }
            else if (channel == 0)
            {
                int index = ctl[i]->GetIndex();
                if (index >= 0)
                {
                    PostMessage(hwnd, WM_SET_PARAMETER, index, GetPar(index) * MAXPARVALUE);
                }
            }
            break;
        }
    }
    this->prevpoint  = point;
    SetFocus(hwnd);
}

void CEditor::OnLButtonUp()
{
    if (changingControl && channel == 0)
    {
        int index = ctl[cID]->GetIndex();
        if (index >= 0)
        {
            PostMessage(hwnd, WM_SET_PARAMETER, index, GetPar(index) * MAXPARVALUE);
        }
    }
    changingControl = FALSE;
    ReleaseCapture();
}

void CEditor::OnRButtonDown()
{
}

void CEditor::OnMouseMove(POINT point)
{
    if (changingControl)
    {
        char str[TEXT_SIZE];
        int value = -(point.y - this->prevpoint.y) + (point.x - this->prevpoint.x);
        this->prevpoint = point;
        if (ctl[cID]->IncreaseValue(value) == true)
        {
            CMapper::GetDisplayValue(this->synthesizer, this->channel, ctl[cID]->GetIndex(), ctl[cID]->GetType(), str);
            lcd->SetText(1, str);
        }
    }
}

void CEditor::OnMouseWheel(POINT point, int delta)
{
    char str[TEXT_SIZE];
    for (int i=0;i<GUI_CONTROLS;i++)
    {
        if (ctl[i]->IsMouseOver(point))
        {
            if (ctl[i]->IsKnob() == true)
            {
                ctl[i]->GetName(str);
                lcd->SetText(0,str);
                if (ctl[i]->IncreaseValue(delta) == true)
                {
                    CMapper::GetDisplayValue(this->synthesizer, this->channel, ctl[i]->GetIndex(), ctl[i]->GetType(), str);
                    lcd->SetText(1,str);
                    if (channel == 0)
                    {
                        int index = ctl[i]->GetIndex();
                        if (index >= 0)
                        {
                            PostMessage(hwnd, WM_SET_PARAMETER, index, lrintf(GetPar(index) * MAXPARVALUE));
                        }
                    }
                }
            }
            break;
        }
    }
    SetFocus(hwnd);
}

void CEditor::OnPaint(HDC dc, RECT rect)
{
    int w = rect.right - rect.left;
    int h = rect.bottom - rect.top;
    BitBlt(dc, rect.left, rect.top, w, h, hdcMem, rect.left, rect.top, SRCCOPY);
}

void CEditor::OnTimer(HWND hWnd)
{
    for (int i=0;i<GUI_CONTROLS;i++)
    {
        ctl[i]->Update();
    }
    if (synthesizer->HasChanges())
    {
        if (!synthesizer->GetBankMode())
        {
            if (!synthesizer->GetStandBy(channel))
            {
                ProgramChanged();
            }
            else
            {
                ProgramChangedWaiting();
            }
        }
        changingControl = FALSE;
    }
    // hover feature
    POINT point;
    GetCursorPos(&point);
    ScreenToClient(hwnd, &point);
    if (point.x < 0 || point.y < 0 || changingControl)
    {
        return;
    }
    for (int i=0;i<GUI_CONTROLS;i++)
    {
        if (ctl[i]->IsMouseOver(point) && cID != i)
        {
            cID = i;
            char str[TEXT_SIZE];
            ctl[i]->GetName(str);
            lcd->SetText(0,str);
            CMapper::GetDisplayValue(this->synthesizer, this->channel, ctl[i]->GetIndex(), ctl[i]->GetType(), str);
            lcd->SetText(1,str);
            break;
        }
    }
}

void CEditor::SetPar(int index, float value)
{
    const char CHANNEL = 0;
    for (int i = 0; i < GUI_CONTROLS; i++)
    {
        if (ctl[i]->GetIndex() == index)
        {
            int type = ctl[i]->GetType();
            float fvalue = CMapper::IntValueToFloatValue(this->synthesizer, CHANNEL, index, type, lrintf(value * MAXPARVALUE));
            synthesizer->SetPar(CHANNEL, index, fvalue);
            break;
        }
    }
}

float CEditor::GetPar(int index)
{
    const char CHANNEL = 0;
    for (int i = 0; i < GUI_CONTROLS; i++)
    {
        if (ctl[i]->GetIndex() == index)
        {
            int type = ctl[i]->GetType();
            float fvalue = synthesizer->GetPar(CHANNEL, index);
            float vstvalue = (float)CMapper::FloatValueToIntValue(this->synthesizer, CHANNEL, index, type, fvalue);
            return vstvalue / MAXPARVALUE;
        }
    }
}

void CEditor::GetParLabel(int index, char* label)
{
    StringCchCopyA(label, TEXT_SIZE, "");
}

void CEditor::GetParDisplay(int index, char* text)
{
    const char CHANNEL = 0;
    for (int i = 0; i < GUI_CONTROLS; i++)
    {
        if (ctl[i]->GetIndex() == index)
        {
            int type = ctl[i]->GetType();
            CMapper::GetDisplayValue(this->synthesizer, CHANNEL, index, type, text);
            return;
        }
    }
    StringCchCopyA(text, TEXT_SIZE, "not found");
}

void CEditor::GetParName(int index, char* text)
{
    for (int i = 0; i < GUI_CONTROLS; i++)
    {
        if (ctl[i]->GetIndex() == index)
        {
            ctl[i]->GetName(text);
            return;
        }
    }
    StringCchCopyA(text, TEXT_SIZE, "not found");
}
