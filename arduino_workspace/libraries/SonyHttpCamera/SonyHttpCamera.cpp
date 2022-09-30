#include "SonyHttpCamera.h"

DebuggingSerial* SonyHttpCamera::dbgser_important;
DebuggingSerial* SonyHttpCamera::dbgser_states;
DebuggingSerial* SonyHttpCamera::dbgser_events;
DebuggingSerial* SonyHttpCamera::dbgser_rx;
DebuggingSerial* SonyHttpCamera::dbgser_tx;
DebuggingSerial* SonyHttpCamera::dbgser_devprop_dump;
DebuggingSerial* SonyHttpCamera::dbgser_devprop_change;

SonyHttpCamera::SonyHttpCamera()
{
    tbl_iso = NULL;
    tbl_shutterspd = NULL;

    dbgser_important       = new DebuggingSerial(&Serial);
    dbgser_states          = new DebuggingSerial(&Serial);
    dbgser_events          = new DebuggingSerial(&Serial);
    dbgser_rx              = new DebuggingSerial(&Serial);
    dbgser_tx              = new DebuggingSerial(&Serial);
    dbgser_devprop_dump    = new DebuggingSerial(&Serial);
    dbgser_devprop_change  = new DebuggingSerial(&Serial);

    dbgser_important->enabled = false;
    dbgser_states->   enabled = true;
    dbgser_events->   enabled = false;
    dbgser_rx->       enabled = false;
    dbgser_tx->       enabled = false;
    dbgser_devprop_dump->  enabled = false;
    dbgser_devprop_change->enabled = false;

    begin(0);
}

void SonyHttpCamera::begin(uint32_t ip, WiFiUDP* sock)
{
    uint32_t now = millis();
    ip_addr = ip;
    ssdp_udp = sock;
    init_retries = 0;
    error_cnt = 0;
    event_api_version = 3;
    is_movierecording_v = false;
    is_manuallyfocused_v = 0;
    is_focused = false;
    has_focus_status = false;
    req_id = 1;
    rx_buff_idx = 0;
    if (rx_buff != NULL) {
        rx_buff[0] = 0;
    }
    url_buffer[0] = 0;
    last_poll_time = 0;
    if (state != SHCAMSTATE_FORBIDDEN && ip != 0)
    {
        state = SHCAMSTATE_WAIT;
        state_after_wait = SHCAMSTATE_CONNECTING;
        start_time = now;
        wait_until = now + 1000;
        dbgser_states->printf("httpcam hello IP %08X\r\n", ip);
    }
    else if (ip == 0)
    {
        state = SHCAMSTATE_NONE;
    }

    zoom_state = 0;
    zoom_time = 0;
    poll_delay = 500;

    #if 0
    if (tbl_iso != NULL) {
        free(tbl_iso);
    }
    if (tbl_shutterspd != NULL) {
        free(tbl_shutterspd);
    }
    #endif
}

bool SonyHttpCamera::parse_event(char* data, int32_t maxlen)
{
    char res_buff[64] = {0};
    signed int i, j, k;
    bool ret = false;
    bool found;

    if (maxlen <= 0) {
        maxlen = rx_buff_idx;
    }

    found = scan_json_for_key(rx_buff, maxlen, "cameraStatus", &i, &j, (char*)res_buff, 64);
    if (found) {
        is_movierecording_v = (memcmp(res_buff, "Movie", 5) == 0);
        dbgser_devprop_dump->printf("httpcam event key \"cameraStatus\" = \"%s\"\r\n", res_buff);
        ret |= true;
        event_found_flag |= (1 << 0);
    }

    found = scan_json_for_key(rx_buff, maxlen, "focusStatus", &i, &j, (char*)res_buff, 64);
    if (found) {
        strcpy(str_focusstatus, res_buff);
        dbgser_devprop_dump->printf("httpcam event key \"focusStatus\" = \"%s\"\r\n", res_buff);
        is_focused = (memcmp(res_buff, "Focused", 7) == 0);
        has_focus_status = true;
        ret |= true;
        event_found_flag |= (1 << 1);
    }

    found = scan_json_for_key(rx_buff, maxlen, "currentIsoSpeedRate", &i, &j, (char*)res_buff, 64);
    if (found && strlen(res_buff) > 0) {
        strcpy(str_iso, res_buff);
        dbgser_devprop_dump->printf("httpcam event key \"currentIsoSpeedRate\" = \"%s\"\r\n", res_buff);
        ret |= true;
        event_found_flag |= (1 << 2);
    }

    if (tbl_iso == NULL) {
        found = scan_json_for_key(rx_buff, maxlen, "isoSpeedRateCandidates", &i, &j, NULL, 0);
        if (found) {
            k = j - i + 3;
            tbl_iso = (char*)malloc(k);
            found = scan_json_for_key(rx_buff, maxlen, "isoSpeedRateCandidates", &i, &j, (char*)tbl_iso, k - 1);
            dbgser_devprop_dump->printf("httpcam event key \"isoSpeedRateCandidates\" = \"%s\"\r\n", tbl_iso);
            ret |= true;
            event_found_flag |= (1 << 3);
        }
    }

    found = scan_json_for_key(rx_buff, maxlen, "currentShutterSpeed", &i, &j, (char*)res_buff, 64);
    if (found && strlen(res_buff) > 0) {
        strcpy(str_shutterspd, res_buff);
        strcpy_no_slash(str_shutterspd_clean, str_shutterspd);
        dbgser_devprop_dump->printf("httpcam event key \"currentShutterSpeed\" = \"%s\"\r\n", res_buff);
        ret |= true;
        event_found_flag |= (1 << 4);
    }

    if (tbl_shutterspd == NULL) {
        found = scan_json_for_key(rx_buff, maxlen, "shutterSpeedCandidates", &i, &j, NULL, 0);
        if (found) {
            k = j - i + 3;
            tbl_shutterspd = (char*)malloc(k);
            found = scan_json_for_key(rx_buff, maxlen, "shutterSpeedCandidates", &i, &j, (char*)tbl_shutterspd, k - 1);
            dbgser_devprop_dump->printf("httpcam event key \"shutterSpeedCandidates\" = \"%s\"\r\n", tbl_shutterspd);
            ret |= true;
            event_found_flag |= (1 << 5);
        }
    }

    found = scan_json_for_key(rx_buff, maxlen, "currentFocusMode", &i, &j, (char*)res_buff, 64);
    if (found && strlen(res_buff) > 0) {
        // warning, it seems like this item is missing, even from my a6600
        is_manuallyfocused_v = (memcmp(res_buff, "AF" , 2) == 0) ? SHCAM_FOCUSMODE_AF : is_manuallyfocused_v;
        is_manuallyfocused_v = (memcmp(res_buff, "MF" , 2) == 0) ? SHCAM_FOCUSMODE_MF : is_manuallyfocused_v;
        is_manuallyfocused_v = (memcmp(res_buff, "DMF", 3) == 0) ? SHCAM_FOCUSMODE_MF : is_manuallyfocused_v;
        if (is_manuallyfocused_v == SHCAM_FOCUSMODE_AF) {
            strcpy(str_afmode, res_buff);
        }
        dbgser_devprop_dump->printf("httpcam event key \"currentFocusMode\" = \"%s\"\r\n", res_buff);
        ret |= true;
        event_found_flag |= (1 << 6);
    }

    found = scan_json_for_key(rx_buff, maxlen, "currentShootMode", &i, &j, (char*)res_buff, 64);
    if (found && strlen(res_buff) > 0) {
        if (memcmp("still", res_buff, 5) == 0) {
            shoot_mode == SHOOTMODE_STILLS;
        }
        else if (memcmp("movie", res_buff, 5) == 0) {
            shoot_mode == SHOOTMODE_MOVIE;
        }
        dbgser_devprop_dump->printf("httpcam event key \"currentShootMode\" = \"%s\"\r\n", res_buff);
        ret |= true;
        event_found_flag |= (1 << 7);
    }

    found = scan_json_for_key(rx_buff, maxlen, "error", &i, &j, (char*)res_buff, 64);
    if (found) {
        dbgser_devprop_dump->printf("httpcam event key \"res_buff\" = \"%s\"\r\n", res_buff);
        int eno;
        if (parse_json_err_num((const char*)res_buff, &eno))
        {
            // check error number, 14 means unsupported, downgrade the version number for event fetching
            if (eno == 14 && event_api_version > 0) {
                event_api_version--;
            }
        }
    }
    else {
        if (event_found_flag != 0) {
            critical_error_cnt = 0;
            error_cnt = 0;
        }
    }

    #if 0
    dbgser_devprop_dump->printf(rx_buff);
    dbgser_devprop_dump->println();
    #endif

    return ret;
}

#ifdef SHCAM_USE_ASYNC
void SonyHttpCamera::eventRequestCb(void* optParm, AsyncHTTPRequest* req, int readyState)
{
    SonyHttpCamera* cam = (SonyHttpCamera*)optParm;

    if (cam->state == SHCAMSTATE_FORBIDDEN) {
        return;
    }

    dbgser_rx->printf("httpcam rx eventRequestCb %d %u\r\n", readyState, req->available());

    if (readyState == readyStateDone)
    {
        int32_t chunk;
        while ((chunk = req->available()) > 0)
        {
            int32_t r = read_in_chunk(req, chunk, cam->rx_buff, &(cam->rx_buff_idx));
            if (r == 0) {
                break;
            }
            cam->parse_event(cam->rx_buff, cam->rx_buff_idx);
        }

        int respcode;
        if ((respcode = req->responseHTTPcode()) == 200)
        {
            dbgser_devprop_dump->printf("\r\nhttpcam polled event 0x%08X\r\n", cam->event_found_flag);
            if (strlen(cam->str_focusstatus) <= 0) {
                if (cam->event_api_version >= 1) {
                    cam->event_api_version--;
                }
            }
            cam->error_cnt = 0;
            if (cam->state < SHCAMSTATE_READY) {
                dbgser_states->printf("httpcam connected\r\n");
                if (cam->cb_onConnect != NULL) {
                    cam->cb_onConnect();
                }
            }
            cam->state = SHCAMSTATE_READY;
        }
        else
        {
            dbgser_states->printf("httpcam event resp err %d\r\n", respcode);
            cam->error_cnt++;
        }
        cam->request_close();
    }
}

void SonyHttpCamera::eventDataCb(void* optParm, AsyncHTTPRequest* req, int avail)
{
    SonyHttpCamera* cam = (SonyHttpCamera*)optParm;

    if (cam->state == SHCAMSTATE_FORBIDDEN) {
        return;
    }
    if (req->available() <= 0) {
        dbgser_rx->printf("httpcam rx eventDataCb %u\r\n", req->available());
        return;
    }
    dbgser_rx->printf("httpcam rx eventDataCb %u\r\n", req->available());

    int32_t chunk;
    while ((chunk = req->available()) > 0)
    {
        int32_t r = read_in_chunk(req, chunk, cam->rx_buff, &(cam->rx_buff_idx));
        if (r == 0) {
            break;
        }
        cam->parse_event(cam->rx_buff, cam->rx_buff_idx);
    }
}
#endif

void SonyHttpCamera::get_event()
{
    rx_buff_idx = 0;
    event_found_flag = 0;
    #ifdef SHCAM_USE_ASYNC
    bool openres = request_prep("POST", service_url, "application/json", eventRequestCb, eventDataCb);
    if (openres)
    #endif
    {
        sprintf(cmd_buffer, "{ \"method\": \"getEvent\", \"params\": [false], \"id\": %u, \"version\": \"1.%u\" }", req_id, event_api_version);
        dbgser_tx->printf("httpcam get_event json: %s\r\n", cmd_buffer);
        cmd_send(cmd_buffer, NULL, false);
        req_id++;
        state = SHCAMSTATE_POLLING + 1;
        last_poll_time = millis();
    }
    #ifdef SHCAM_USE_ASYNC
    else
    {
        dbgser_tx->printf("httpcam unable to send get_event\r\n");
        state = SHCAMSTATE_WAIT;
        state_after_wait = SHCAMSTATE_INIT_DONE2;
        wait_until = millis() + 500;
    }
    #endif
}

void SonyHttpCamera::poll()
{
    yield();
    #ifndef SHCAM_USE_ASYNC
    if (state == SHCAMSTATE_POLLING + 1)
    {
        bool no_len = http_content_len < 0;
        WiFiClient* cli = NULL;
        if (httpclient.connected())
        {
            cli = httpclient.getStreamPtr();
            int avail;
            while ((avail = cli->available()) > 0)
            {
                int r = read_in_chunk(cli, avail, rx_buff, &rx_buff_idx);
                if (http_content_len >= 0) {
                    http_content_len -= r;
                }
                if (r == 0) {
                    break;
                }
                //parse_event(rx_buff, rx_buff_idx);
            }
            //dbgser_states->printf("httpcam event read with rem=%d\r\n", http_content_len);
        }
        if ((no_len && httpclient.connected() == false) || (no_len == false && http_content_len <= 0) || httpclient.connected() == false)
        {
            dbgser_rx->println();
            parse_event(rx_buff, rx_buff_idx);
            if (cli != NULL) {
                while (cli->available() > 0) {
                    cli->read();
                }
            }
            if (httpclient.connected()) {
                httpclient.end();
            }
            dbgser_states->printf("httpcam finalizing poll, evt 0x%08X\r\n", event_found_flag);
            state = SHCAMSTATE_POLLING;
        }
    }
    #endif
}

void SonyHttpCamera::task()
{
    if (state == SHCAMSTATE_FORBIDDEN) {
        return;
    }

    uint32_t now = millis();

    poll();

    if (state == SHCAMSTATE_WAIT && state_after_wait == SHCAMSTATE_CONNECTING && now < wait_until && ip_addr != 0)
    {
        ssdp_poll(ssdp_udp);
        return;
    }

    if (state == SHCAMSTATE_WAIT && now >= wait_until) {
        state = state_after_wait;
        if (state == SHCAMSTATE_CONNECTING && ip_addr != 0) {
            dbgser_states->println("httpcam starting SSDP");
            if (ssdp_udp == NULL) {
                ssdp_udp = new WiFiUDP();
            }
            ssdp_start(ssdp_udp);
            state = SHCAMSTATE_INIT_SSDP;
            wait_until = now + 2000;
            init_retries = 0;
            return;
        }
    }

    if (state == SHCAMSTATE_INIT_SSDP)
    {
        ssdp_poll(ssdp_udp);
    }

    #ifndef SHCAM_USE_ASYNC
    if (state == SHCAMSTATE_INIT_GETDD && init_retries > 0)
    {
        get_dd_xml();
    }
    #endif

    if (state == SHCAMSTATE_INIT_GOTDD)
    {
        #if 0
        state = SHCAMSTATE_INIT_ACCESSCONTROL;
        #else
        //state = SHCAMSTATE_INIT_GETVERSION;
        state = SHCAMSTATE_INIT_STARTRECMODE;
        #endif
        init_retries = 0;
    }

    if ((state == SHCAMSTATE_INIT_ACCESSCONTROL || state == SHCAMSTATE_INIT_GETVERSION || state == SHCAMSTATE_INIT_GETAPILIST || state == SHCAMSTATE_INIT_STARTRECMODE || state == SHCAMSTATE_INIT_SETCAMFUNC) && (state & 1) == 0
        #ifdef SHCAM_USE_ASYNC
        && (httpreq->readyState() == readyStateUnsent || httpreq->readyState() == readyStateDone)
        #endif
        )
    {
        bool success;
        #ifdef SHCAM_USE_ASYNC
        bool openres = request_prep("POST", (state == SHCAMSTATE_INIT_ACCESSCONTROL) ? access_url : service_url, "application/json", initRequestCb, NULL);
        if (openres)
        #endif
        {
            #if 0
            if (state == SHCAMSTATE_INIT_ACCESSCONTROL) {
                sprintf(cmd_buffer, "{\"version\": \"1.0\", \"params\": [{\"developerName\": \"\", \"sg\": \"\", \"methods\": \"\", \"developerID\": \"\"}], \"method\": \"actEnableMethods\", \"id\": %u}", req_id);
                success = cmd_send(cmd_buffer, access_url, true);
                endif
                req_id++;
                dbgser_tx->printf("httpcam init accessControl\r\n");
            }
            else
            #endif
            if (state == SHCAMSTATE_INIT_GETVERSION) {
                sprintf(cmd_buffer, cmd_generic_fmt, "getVersions", req_id);
                success = cmd_send(cmd_buffer);
                req_id++;
                dbgser_tx->printf("httpcam init getVersions\r\n");
            }
            else if (state == SHCAMSTATE_INIT_GETAPILIST) {
                sprintf(cmd_buffer, cmd_generic_fmt, "getAvailableApiList", req_id);
                success = cmd_send(cmd_buffer);
                req_id++;
                dbgser_tx->printf("httpcam init getAvailableApiList\r\n");
            }
            else if (state == SHCAMSTATE_INIT_STARTRECMODE) {
                sprintf(cmd_buffer, cmd_generic_fmt, "startRecMode", req_id);
                success = cmd_send(cmd_buffer);
                req_id++;
                dbgser_tx->printf("httpcam init startRecMode\r\n");
            }
            else if (state == SHCAMSTATE_INIT_SETCAMFUNC) {
                sprintf(cmd_buffer, cmd_generic_fmt, "cameraFunction", req_id);
                success = cmd_send(cmd_buffer);
                req_id++;
                dbgser_tx->printf("httpcam init cameraFunction\r\n");
            }
            #ifdef SHCAM_USE_ASYNC
            state++; // set the flag for waiting
            #else
            if (success)
            {
                state += 2;
                dbgser_states->printf("httpcam init next state %u\r\n", state);
            }
            else
            {
                init_retries++;
                if (init_retries > 2) {
                    state += 2;
                    dbgser_states->printf("httpcam init err %d, giving up, next state %u\r\n", last_http_resp_code, state);
                    init_retries = 0;
                }
                else {
                    dbgser_states->printf("httpcam init err %d, state %u, try %u\r\n", last_http_resp_code, state, init_retries);
                }
            }
            #endif
            last_poll_time = millis();
        }
    }

    if ((state > SHCAMSTATE_INIT_DONE1 && state < SHCAMSTATE_POLLING) && (state & 1) == 0
        #ifdef SHCAM_USE_ASYNC
        && (httpreq->readyState() == readyStateUnsent || httpreq->readyState() == readyStateDone)
        #endif
        )
    {
        dbgser_states->print("httpcam performing first getEvent\r\n");
        get_event();
    }

    if ((state == SHCAMSTATE_READY || state == SHCAMSTATE_POLLING))
    {
        if ((now - last_poll_time) > poll_delay)
        {
            get_event();
        }
        else if ((now - zoom_time) > 5000 && zoom_time != 0 && zoom_state != 0)
        {
            cmd_ZoomStop();
        }
    }

    if (error_cnt > 10) {
        state = SHCAMSTATE_FAILED;
        if (cb_onDisconnect != NULL) {
            cb_onDisconnect();
        }
    }
}

#ifdef SHCAM_USE_ASYNC
void SonyHttpCamera::genericRequestCb(void* optParm, AsyncHTTPRequest* req, int readyState)
{
    SonyHttpCamera* cam = (SonyHttpCamera*)optParm;

    if (cam->state == SHCAMSTATE_FORBIDDEN) {
        return;
    }

    if (readyState == readyStateDone) 
    {
        int rcode;
        if ((rcode = req->responseHTTPcode()) == 200)
        {
            cam->state &= 0xFE; // only clear the wait flag
            cam->error_cnt = 0;
        }
        else
        {
            cam->error_cnt++;
        }
        cam->request_close();
    }
}
#endif

void SonyHttpCamera::set_debugflags(uint32_t x)
{
    debug_flags = x;
    dbgser_states->        enabled = ((x & DEBUGFLAG_STATES        ) != 0);
    dbgser_events->        enabled = ((x & DEBUGFLAG_EVENTS        ) != 0);
    dbgser_rx->            enabled = ((x & DEBUGFLAG_RX            ) != 0);
    dbgser_tx->            enabled = ((x & DEBUGFLAG_TX            ) != 0);
    dbgser_devprop_dump->  enabled = ((x & DEBUGFLAG_DEVPROP_DUMP  ) != 0);
    dbgser_devprop_change->enabled = ((x & DEBUGFLAG_DEVPROP_CHANGE) != 0);
}

void SonyHttpCamera::test_debug_msg(const char* s)
{
    dbgser_states->print(s);
}
