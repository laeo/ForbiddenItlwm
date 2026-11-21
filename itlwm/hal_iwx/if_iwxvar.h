/*
* Copyright (C) 2020  钟先耀
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*/
/*    $OpenBSD: if_iwxvar.h,v 1.13 2020/10/11 07:05:28 mpi Exp $    */

/*
 * Copyright (c) 2014 genua mbh <info@genua.de>
 * Copyright (c) 2014 Fixup Software Ltd.
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

/*-
 * Based on BSD-licensed source modules in the Linux iwlwifi driver,
 * which were used as the reference documentation for this implementation.
 *
 ******************************************************************************
 *
 * This file is provided under a dual BSD/GPLv2 license.  When using or
 * redistributing this file, you may do so under either license.
 *
 * GPL LICENSE SUMMARY
 *
 * Copyright(c) 2017 Intel Deutschland GmbH
 * Copyright(c) 2018 - 2019 Intel Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * BSD LICENSE
 *
 * Copyright(c) 2017 Intel Deutschland GmbH
 * Copyright(c) 2018 - 2019 Intel Corporation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *  * Neither the name Intel Corporation nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *****************************************************************************
 */

/*-
 * Copyright (c) 2007-2010 Damien Bergamini <damien.bergamini@free.fr>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */
#include <net80211/ieee80211_var.h>
#include <net80211/ieee80211_amrr.h>
#include <net80211/ieee80211_mira.h>
#include <net80211/ieee80211_radiotap.h>

#include <IOKit/network/IOMbufMemoryCursor.h>
#include <IOKit/IODMACommand.h>
#include <HAL/ItlTx.hpp>

#define IWL_CFG_ANY (~0)

#define IWL_CFG_MAC_TYPE_PU        0x31
#define IWL_CFG_MAC_TYPE_PNJ        0x32
#define IWL_CFG_MAC_TYPE_TH        0x32
#define IWL_CFG_MAC_TYPE_QU        0x33
#define IWL_CFG_MAC_TYPE_QUZ        0x35
#define IWL_CFG_MAC_TYPE_QNJ        0x36
#define IWL_CFG_MAC_TYPE_SO        0x37
#define IWL_CFG_MAC_TYPE_SNJ        0x42
#define IWL_CFG_MAC_TYPE_SOF        0x43
#define IWL_CFG_MAC_TYPE_MA        0x44
#define IWL_CFG_MAC_TYPE_BZ        0x46
/* Scarborough (Wi-Fi 7 BE2xx on Panther Lake and later). Linux: IWL_CFG_MAC_TYPE_SC. */
#define IWL_CFG_MAC_TYPE_SC        0x48

#define IWL_CFG_RF_TYPE_TH        0x105
#define IWL_CFG_RF_TYPE_TH1        0x108
#define IWL_CFG_RF_TYPE_JF2        0x105
#define IWL_CFG_RF_TYPE_JF1        0x108
#define IWL_CFG_RF_TYPE_HR2        0x10A
#define IWL_CFG_RF_TYPE_HR1        0x10C
#define IWL_CFG_RF_TYPE_GF        0x10D
#define IWL_CFG_RF_TYPE_MR        0x110
#define IWL_CFG_RF_TYPE_FM        0x112

#define IWL_CFG_RF_ID_TH        0x1
#define IWL_CFG_RF_ID_TH1        0x1
#define IWL_CFG_RF_ID_JF        0x3
#define IWL_CFG_RF_ID_JF1        0x6
#define IWL_CFG_RF_ID_JF1_DIV        0xA
#define IWL_CFG_RF_ID_HR        0x7
#define IWL_CFG_RF_ID_HR1        0x4

#define IWL_CFG_NO_160            0x1
#define IWL_CFG_160            0x0

#define IWL_CFG_CORES_BT        0x0
#define IWL_CFG_CORES_BT_GNSS        0x5

#define IWL_CFG_NO_CDB            0x0
#define IWL_CFG_CDB            0x1

#define IWL_SUBDEVICE_RF_ID(subdevice)    ((u16)((subdevice) & 0x00F0) >> 4)
#define IWL_SUBDEVICE_NO_160(subdevice)    ((u16)((subdevice) & 0x0200) >> 9)
#define IWL_SUBDEVICE_CORES(subdevice)    ((u16)((subdevice) & 0x1C00) >> 10)

/* HW REV */
#define CSR_HW_REV_TYPE(_val)          (((_val) & 0x000FFF0) >> 4)

/* HW RFID */
#define CSR_HW_RFID_FLAVOR(_val)       (((_val) & 0x000000F) >> 0)
#define CSR_HW_RFID_DASH(_val)         (((_val) & 0x00000F0) >> 4)
#define CSR_HW_RFID_STEP(_val)         (((_val) & 0x0000F00) >> 8)
#define CSR_HW_RFID_TYPE(_val)         (((_val) & 0x0FFF000) >> 12)
#define CSR_HW_RFID_IS_CDB(_val)       (((_val) & 0x10000000) >> 28)
#define CSR_HW_RFID_IS_JACKET(_val)    (((_val) & 0x20000000) >> 29)

#define CSR_HW_REV_TYPE_MSK        (0x000FFF0)
#define CSR_HW_REV_TYPE_NONE        (0x00001F0)
#define CSR_HW_REV_TYPE_QNJ        (0x0000360)
#define CSR_HW_REV_TYPE_QNJ_B0        (0x0000364)
#define CSR_HW_REV_TYPE_QU_B0        (0x0000334)
#define CSR_HW_REV_TYPE_QU_C0        (0x0000338)
#define CSR_HW_REV_TYPE_QUZ        (0x0000354)
#define CSR_HW_REV_TYPE_HR_CDB        (0x0000340)
#define CSR_HW_REV_TYPE_SO        (0x0000370)
#define CSR_HW_REV_TYPE_TY        (0x0000420)

/* RF_ID value */
#define CSR_HW_RF_ID_TYPE_JF        (0x00105100)
#define CSR_HW_RF_ID_TYPE_HR        (0x0010A000)
#define CSR_HW_RF_ID_TYPE_HR1        (0x0010c100)
#define CSR_HW_RF_ID_TYPE_HRCDB        (0x00109F00)
#define CSR_HW_RF_ID_TYPE_GF        (0x0010D000)
#define CSR_HW_RF_ID_TYPE_GF4        (0x0010E000)

/* HW_RF CHIP ID  */
#define CSR_HW_RF_ID_TYPE_CHIP_ID(_val) (((_val) >> 12) & 0xFFF)

/* HW_RF CHIP STEP  */
#define CSR_HW_RF_STEP(_val) (((_val) >> 8) & 0xF)

struct iwx_rx_radiotap_header {
	struct ieee80211_radiotap_header wr_ihdr;
	uint64_t	wr_tsft;
	uint8_t		wr_flags;
	uint8_t		wr_rate;
	uint16_t	wr_chan_freq;
	uint16_t	wr_chan_flags;
	int8_t		wr_dbm_antsignal;
	int8_t		wr_dbm_antnoise;
} __packed;

#define IWX_RX_RADIOTAP_PRESENT						\
	((1 << IEEE80211_RADIOTAP_TSFT) |				\
	 (1 << IEEE80211_RADIOTAP_FLAGS) |				\
	 (1 << IEEE80211_RADIOTAP_RATE) |				\
	 (1 << IEEE80211_RADIOTAP_CHANNEL) |				\
	 (1 << IEEE80211_RADIOTAP_DBM_ANTSIGNAL) |			\
	 (1 << IEEE80211_RADIOTAP_DBM_ANTNOISE))

struct iwx_tx_radiotap_header {
	struct ieee80211_radiotap_header wt_ihdr;
	uint8_t		wt_flags;
	uint8_t		wt_rate;
	uint16_t	wt_chan_freq;
	uint16_t	wt_chan_flags;
} __packed;

#define IWX_TX_RADIOTAP_PRESENT						\
	((1 << IEEE80211_RADIOTAP_FLAGS) |				\
	 (1 << IEEE80211_RADIOTAP_RATE) |				\
	 (1 << IEEE80211_RADIOTAP_CHANNEL))

/*
 * Scarborough (sc-a0) firmware carries 70 SEC_RT sections, more than the 60 the
 * Qu/So/Ma parts use. Linux keeps a 96 entry table (IWL_UCODE_SECTION_MAX is sized
 * from the biggest firmware, not from the oldest). With 60 the parse stops at
 * section 61 with EINVAL and the driver reports "firmware parse error 22".
 */
#define IWX_UCODE_SECT_MAX 96

/*
 * fw_status is used to determine if we've already parsed the firmware file
 *
 * In addition to the following, status < 0 ==> -error
 */
#define IWX_FW_STATUS_NONE		0
#define IWX_FW_STATUS_INPROGRESS	1
#define IWX_FW_STATUS_DONE		2

enum iwx_ucode_type {
	IWX_UCODE_TYPE_REGULAR,
	IWX_UCODE_TYPE_INIT,
	IWX_UCODE_TYPE_WOW,
	IWX_UCODE_TYPE_REGULAR_USNIFFER,
	IWX_UCODE_TYPE_MAX
};

struct iwx_fw_onesect {
    void *fws_data;
    uint32_t fws_len;
    uint32_t fws_devoff;
};

struct iwx_fw_sects {
    struct iwx_fw_onesect fw_sect[IWX_UCODE_SECT_MAX];
    size_t fw_totlen;
    int fw_count;
};

struct iwx_fw_info {
	void *fw_rawdata;
	size_t fw_rawsize;
    void *pnvm_rawdata;
    size_t pnvm_rawsize;
	/*
	 * PNVM payload embedded in the firmware image itself
	 * (IWL_UCODE_TLV_PNVM_DATA = 74). Points into the decompressed
	 * fw_rawdata buffer, so it lives as long as fw_rawdata does.
	 */
	const uint8_t *pnvm_embedded;
	size_t pnvm_embedded_len;
	int fw_status;

	struct iwx_fw_sects fw_sects[IWX_UCODE_TYPE_MAX];

	/* FW debug data parsed for driver usage */
	int dbg_dest_tlv_init;
	uint8_t *dbg_dest_ver;
	uint8_t n_dest_reg;
	struct iwx_fw_dbg_dest_tlv_v1 *dbg_dest_tlv_v1;

	struct iwx_fw_dbg_conf_tlv *dbg_conf_tlv[IWX_FW_DBG_CONF_MAX];
	size_t dbg_conf_tlv_len[IWX_FW_DBG_CONF_MAX];
	struct iwx_fw_dbg_trigger_tlv *dbg_trigger_tlv[IWX_FW_DBG_TRIGGER_MAX];
	size_t dbg_trigger_tlv_len[IWX_FW_DBG_TRIGGER_MAX];
	struct iwx_fw_dbg_mem_seg_tlv *dbg_mem_tlv;
	size_t n_mem_tlv;
};

struct iwx_nvm_data {
	int n_hw_addrs;
	uint8_t hw_addr[ETHER_ADDR_LEN];

	int sku_cap_band_24GHz_enable;
	int sku_cap_band_52GHz_enable;
	int sku_cap_11n_enable;
    int sku_cap_11ac_enable;
    int sku_cap_11ax_enable;
	int sku_cap_amt_enable;
	int sku_cap_ipan_enable;
	int sku_cap_mimo_disable;
    int lar_enabled;
    bool vht160_supported;

	uint8_t valid_tx_ant, valid_rx_ant;

	uint16_t nvm_version;
};

/* max bufs per tfd the driver will use */
#define IWX_MAX_CMD_TBS_PER_TFD 2

struct iwx_host_cmd {
	const void *data[IWX_MAX_CMD_TBS_PER_TFD];
	struct iwx_rx_packet *resp_pkt;
	size_t resp_pkt_len;
	unsigned long _rx_page_addr;
	uint32_t _rx_page_order;
	int handler_status;

	uint32_t flags;
	uint16_t len[IWX_MAX_CMD_TBS_PER_TFD];
	uint8_t dataflags[IWX_MAX_CMD_TBS_PER_TFD];
	uint32_t id;
};

/*
 * DMA glue is from iwn
 */

struct iwx_dma_info {
#ifdef AIRPORT_VTD
    ItlDmaBuffer mapping;
#endif
	IOBufferMemoryDescriptor* buffer;
    bus_addr_t        paddr;
    void             *vaddr;
    bus_size_t        size;
    IOBufferMemoryDescriptor *bmd;
    IODMACommand *cmd;
};

struct iwx_tx_data {
	bus_dmamap_t	map;
	bus_addr_t	cmd_paddr;
	mbuf_t m;
	struct iwx_node *in;
    int flags;
#define IWX_TXDATA_FLAG_CMD_IS_NARROW  0x01
#define IWX_TXDATA_FLAG_CMD_PENDING    0x02
#define IWX_TXDATA_FLAG_CMD_WAITING    0x04
    uint8_t type;
};

struct iwx_tx_ring {
	struct iwx_dma_info	desc_dma;
	struct iwx_dma_info	cmd_dma;
	struct iwx_dma_info	bc_tbl;
	struct iwx_tfh_tfd	*desc;
	struct iwx_device_cmd	*cmd;
	struct iwx_tx_data	data[IWX_MIN_256_BA_QUEUE_SIZE_GEN3];
    unsigned int    ring_count;
    unsigned int    hi_mark;
    unsigned int    low_mark;
	int			qid;
	int			queued;
	int			cur;
	int			tail;
};

#define IWX_RX_MQ_RING_COUNT	512
/* Linux driver optionally uses 8k buffer */
#define IWX_RBUF_SIZE		4096

struct iwx_rx_data {
	mbuf_t m;
	bus_dmamap_t	map;
};

struct iwx_rx_ring {
	struct iwx_dma_info	free_desc_dma;
	struct iwx_dma_info	stat_dma;
	struct iwx_dma_info	used_desc_dma;
	void			*desc;
	void	        *stat;
	struct iwx_rx_data	data[IWX_RX_MQ_RING_COUNT];
	int			cur;
	/*
	 * Linux keeps rxq->id for the same reason: the free-RBD write pointer on
	 * Bz/SC has to be pushed through HBUS_TARG_WRPTR with the queue number in
	 * bits [31:16] (RX queues are offset by 512). Always 0 on iwx (single
	 * default queue), but explicit beats hard-coding it at the write site.
	 */
	int			qid;
};

#define IWX_FLAG_USE_ICT	0x01	/* using Interrupt Cause Table */
#define IWX_FLAG_RFKILL		0x02	/* radio kill switch is set */
#define IWX_FLAG_SCANNING	0x04	/* scan in progress */
#define IWX_FLAG_MAC_ACTIVE	0x08	/* MAC context added to firmware */
#define IWX_FLAG_BINDING_ACTIVE	0x10	/* MAC->PHY binding added to firmware */
#define IWX_FLAG_STA_ACTIVE	0x20	/* AP added to firmware station table */
#define IWX_FLAG_TE_ACTIVE	0x40	/* time event is scheduled */
#define IWX_FLAG_HW_ERR		0x80	/* hardware error occurred */
#define IWX_FLAG_SHUTDOWN	0x100	/* shutting down; new tasks forbidden */
#define IWX_FLAG_BGSCAN		0x200	/* background scan in progress */
#define IWX_FLAG_TXFLUSH    0x400   /* Tx queue flushing in progress */

struct iwx_ucode_status {
	uint32_t uc_lmac_error_event_table[2];
	uint32_t uc_umac_error_event_table;
	uint32_t uc_log_event_table;
	unsigned int error_event_table_tlv_status;

	int uc_ok;
	int uc_intr;
};

#define IWX_ERROR_EVENT_TABLE_LMAC1	(1 << 0)
#define IWX_ERROR_EVENT_TABLE_LMAC2	(1 << 1)
#define IWX_ERROR_EVENT_TABLE_UMAC	(1 << 2)

#define IWX_CMD_RESP_MAX PAGE_SIZE

/* lower blocks contain EEPROM image and calibration data */
#define IWX_OTP_LOW_IMAGE_SIZE_FAMILY_7000 	16384
#define IWX_OTP_LOW_IMAGE_SIZE_FAMILY_8000	32768

#define IWX_TE_SESSION_PROTECTION_MAX_TIME_MS 1000
#define IWX_TE_SESSION_PROTECTION_MIN_TIME_MS 400

enum IWX_CMD_MODE {
	IWX_CMD_ASYNC		= (1 << 0),
	IWX_CMD_WANT_RESP	= (1 << 1),
	IWX_CMD_SEND_IN_RFKILL	= (1 << 2),
};
enum iwx_hcmd_dataflag {
	IWX_HCMD_DFL_NOCOPY     = (1 << 0),
	IWX_HCMD_DFL_DUP        = (1 << 1),
};

#define IWX_NUM_PAPD_CH_GROUPS	9
#define IWX_NUM_TXP_CH_GROUPS	9

struct iwx_phy_ctxt {
	uint16_t id;
	uint16_t color;
	uint32_t ref;
	struct ieee80211_channel *channel;
};

struct iwx_bf_data {
	int bf_enabled;		/* filtering	*/
	int ba_enabled;		/* abort	*/
	int ave_beacon_signal;
	int last_cqm_event;
};

/**
 * struct iwx_self_init_dram - dram data used by self init process
 * @fw: lmac and umac dram data
 * @lmac_cnt: number of lmac sections in fw image
 * @umac_cnt: number of umac sections in fw image
 * @paging: paging dram data
 * @paging_cnt: number of paging sections needed by fw image
 */
struct iwx_self_init_dram {
	struct iwx_dma_info *fw;
	int lmac_cnt;
    int umac_cnt;
	struct iwx_dma_info *paging;
	int paging_cnt;
};


#define IWX_MAX_TID_COUNT 8
#define IWX_INVALID_QUEUE 0xFFFF

struct iwx_tid_data {
    uint16_t seq_number;
    uint16_t next_reclaimed;
    uint16_t qid;
    uint16_t ssn;
};

#define    INFSLP    UINT64_MAX
#ifdef DELAY
#undef DELAY
#define DELAY IODelay
#endif

struct iwl_cfg_trans_params {
    int integrated;
    int device_family;
    int low_latency_xtal;
    int ltr_delay;
    int xtal_latency;
    int bisr_workaround;
};

struct iwl_cfg {
    struct iwl_cfg_trans_params trans;
    const char *name;
    const char *fwname;
    int device_family;
    int tx_with_siso_diversity;
    int uhb_supported;
    uint8_t max_tx_agg_size;
    uint16_t num_rbds;
};

/**
 * struct iwx_reorder_buffer - per ra/tid/queue reorder buffer
 * @head_sn: reorder window head sn
 * @num_stored: number of mpdus stored in the buffer
 * @buf_size: the reorder buffer size as set by the last addba request
 * @queue: queue of this reorder buffer
 * @last_amsdu: track last ASMDU SN for duplication detection
 * @last_sub_index: track ASMDU sub frame index for duplication detection
 * @reorder_timer: timer for frames are in the reorder buffer. For AMSDU
 *    it is the time of last received sub-frame
 * @removed: prevent timer re-arming
 * @valid: reordering is valid for this queue
 * @consec_oldsn_drops: consecutive drops due to old SN
 * @consec_oldsn_ampdu_gp2: A-MPDU GP2 timestamp to track
 *    when to apply old SN consecutive drop workaround
 * @consec_oldsn_prev_drop: track whether or not an MPDU
 *    that was single/part of the previous A-MPDU was
 *    dropped due to old SN
 */
struct iwx_reorder_buffer {
    uint16_t head_sn;
    uint16_t num_stored;
    uint16_t buf_size;
    uint16_t last_amsdu;
    uint8_t last_sub_index;
    CTimeout *reorder_timer;
    int removed;
    int valid;
    unsigned int consec_oldsn_drops;
    uint32_t consec_oldsn_ampdu_gp2;
    unsigned int consec_oldsn_prev_drop;
#define IWX_AMPDU_CONSEC_DROPS_DELBA    20
};

/**
 * struct iwx_reorder_buf_entry - reorder buffer entry per frame sequence number
 * @frames: list of mbufs stored (A-MSDU subframes share a sequence number)
 * @reorder_time: time the packet was stored in the reorder buffer
 */
struct iwx_reorder_buf_entry {
    struct mbuf_list frames;
    struct timeval reorder_time;
    uint32_t rx_pkt_status;
    int chanidx;
    int is_shortpre;
    uint32_t rate_n_flags;
    uint32_t device_timestamp;
    struct ieee80211_rxinfo rxi;
};

/**
 * struct iwx_rxba_data - BA session data
 * @sta_id: station id
 * @tid: tid of the session
 * @baid: baid of the session
 * @timeout: the timeout set in the addba request
 * @entries_per_queue: # of buffers per queue
 * @last_rx: last rx timestamp, updated only if timeout passed from last update
 * @session_timer: timer to check if BA session expired, runs at 2 * timeout
 * @sc: softc pointer, needed for timer context
 * @reorder_buf: reorder buffer
 * @reorder_buf_data: buffered frames, one entry per sequence number
 */
struct iwx_rxba_data {
    uint8_t sta_id;
    uint8_t tid;
    uint8_t baid;
    uint16_t timeout;
    uint16_t entries_per_queue;
    struct timeval last_rx;
    CTimeout *session_timer;
    struct iwx_softc *sc;
    struct iwx_reorder_buffer reorder_buf;
    struct iwx_reorder_buf_entry entries[IEEE80211_BA_MAX_WINSZ];
};

static inline struct iwx_rxba_data *
iwx_rxba_data_from_reorder_buf(struct iwx_reorder_buffer *buf)
{
    return (struct iwx_rxba_data *)((uint8_t *)buf -
            offsetof(struct iwx_rxba_data, reorder_buf));
}

/**
 * struct iwx_rxq_dup_data - per station per rx queue data
 * @last_seq: last sequence per tid for duplicate packet detection
 * @last_sub_frame: last subframe packet
 */
struct iwx_rxq_dup_data {
    uint16_t last_seq[IWX_MAX_TID_COUNT + 1];
    uint8_t last_sub_frame[IWX_MAX_TID_COUNT + 1];
};

struct iwx_ba_task_data {
    uint32_t        start_tidmask;
    uint32_t        stop_tidmask;
};

struct iwx_softc {
	struct device sc_dev;
	struct ieee80211com sc_ic;
	int (*sc_newstate)(struct ieee80211com *, enum ieee80211_state, int);
	int sc_newstate_pending;
    pci_intr_handle_t ih;

	struct task		init_task; /* NB: not reference-counted */
//	struct refcnt		task_refs;
	struct task newstate_task;
	enum ieee80211_state	ns_nstate;
	int			ns_arg;

	/* Task for firmware BlockAck setup/teardown and its arguments. */
	struct task	ba_task;
    
    struct iwx_ba_task_data    ba_rx;
    struct iwx_ba_task_data    ba_tx;
    
    struct iwx_tid_data sc_tid_data[IWX_MAX_TID_COUNT + 1];//per tid data + mgmt. Look at %iwx_tid_data.

    /* Task for ERP/HT prot/slot-time/EDCA updates. */
    struct task        mac_ctxt_task;
    struct task        chan_ctxt_task;

	bus_space_tag_t sc_st;
	bus_space_handle_t sc_sh;
	bus_size_t sc_sz;
	bus_dma_tag_t sc_dmat;
	pci_chipset_tag_t sc_pct;
	pcitag_t sc_pcitag;
	IOInterruptEventSource *sc_ih;
	int sc_msix;

	/* TX/RX rings. */
	struct iwx_tx_ring txq[IWX_MAX_TVQM_QUEUES];
	struct iwx_rx_ring rxq;
	ItlQueueMask<IWX_MAX_TVQM_QUEUES> qfullmsk;
    struct iwx_tx_ring sc_tvqm_ring;
    int first_data_qid;

	int sc_sf_state;

	/* ICT table. */
	struct iwx_dma_info	ict_dma;
	int			ict_cur;
    uint32_t sku_id[3];

	int sc_hw_rev;
#define IWX_SILICON_A_STEP	0
#define IWX_SILICON_B_STEP	1
#define IWX_SILICON_C_STEP	2
#define IWX_SILICON_D_STEP	3
    int sc_hw_rf_id;
	int sc_hw_id;
    const struct iwl_cfg_trans_params *sc_cfg_params;
    const struct iwl_cfg *sc_cfg;
	int sc_device_family;
	/*
	 * Base of the MAC address CSR block: 0x30 for Bz/SC, 0x380 for
	 * AX210 and older. See IWX_CSR_ADDR_BASE_* in if_iwxreg.h for why
	 * this cannot be a compile-time constant.
	 */
	int sc_mac_addr_from_csr;
	/*
	 * Firmware speaks the MLD API: MAC_CONF_GROUP (0x03) MAC_CONFIG /
	 * LINK_CONFIG / STA_CONFIG instead of MAC_CONTEXT_CMD / BINDING_CONTEXT_CMD
	 * / ADD_STA. Set from IWX_UCODE_TLV_CAPA_MLD_API_SUPPORT (110) once the
	 * capability TLV has been parsed and only for family >= SC.
	 *
	 * On sc-a0 this is the difference between a working association and a
	 * 1 s EWOULDBLOCK on 0x128 (group 1) or 0xE28 (group 14): the firmware
	 * registers MAC_CONFIG/LINK_CONFIG/STA_CONFIG in group 3 and nothing
	 * else, and there is no MAC_CONTEXT command in that set at all.
	 */
	int sc_use_mld_api;
#define IWX_DEVICE_FAMILY_22000	1
#define IWX_DEVICE_FAMILY_AX210	2
/*
 * Placed after AX210 on purpose: every ">= IWX_DEVICE_FAMILY_AX210" test in this
 * file is a gen3/v2 context-info or AX210-and-later register test, which SC
 * (gen2=true, integrated=true, umac_prph_offset=0x300000 in Linux iwl_sc_mac_cfg)
 * also needs. The comparisons that must not include SC are the ones against
 * IWX_DEVICE_FAMILY_22000, and those stay false either way.
 */
#define IWX_DEVICE_FAMILY_SC	3

	/* TLV sets dropped because they pointed past the driver's bit arrays.
	 * Non-zero on sc-a0 is expected and harmless: Linux warns and goes on. */
	/* Raw CSR snapshots taken when ALIVE fails, to tell "firmware never
	 * started" from "firmware started but the notification never arrived". */
	/*
	 * Interrupt-side evidence. ALIVE arrives as an interrupt, so "did an
	 * interrupt ever arrive at all" is the first fork when it does not show
	 * up. These are bumped from the interrupt filter/action and only copied
	 * context: setProperty allocates and takes locks).
	 */
	/*
	 * Immediate write->readback pair for CSR_INT_MASK (0x00c).
	 *
	 * *after* something else touched the register (intrFilter() zeroes it on
	 * every interrupt entry, iwx_restore_interrupts() re-arms with
	 * sc_intmask, and the publish-time read races both). Reading the
	 * register back inside the same function that wrote it, before any ISR
	 * can run, is the only way to tell "the hardware refused a bit" from
	 * "a later software path rewrote it".
	 *
	 * want = the value we asked for, rb = what the register returned,
	 * tag = which call site produced the pair.
	 */
	/*
	 * rxq->stat (the RB status word the RX DMA engine writes back) sampled
	 * once, right after the firmware kick. Compared against the live value at
	 * publish time it says whether the firmware ever handed a packet to the
	 * driver's own ring. ALIVE is delivered as an FH RX packet, so an
	 * unchanged index means either "the firmware never got that far" or "it
	 * wrote but the doorbell/interrupt never reached us"; an advanced index
	 * with alive==0 pins the failure to the interrupt path.
	 */
	/*
	 * Last value actually pushed to the RX free-RBD write pointer, and how
	 * many times it was pushed. iwx_notif_intr() recomputes the pointer from
	 * the firmware's own consumed index at the end of every call:
	 *
	 *     hw = (hw == 0) ? IWX_RX_MQ_RING_COUNT - 1 : hw - 1;
	 *     iwx_rx_inc_wr_ptr(sc, &sc->rxq, hw & ~7);
	 *
	 * With hw = 4 that rounds down to 0, i.e. "zero free RBDs". If the
	 * firmware honours that it stops delivering, and every later H2G command
	 * ack (INIT_EXTENDED_CFG included) is lost - which looks exactly like
	 * "command sent but never acknowledged" (EWOULDBLOCK). Recording the
	 * published value distinguishes that from a genuinely broken TX path.
	 */
	/* Peripheral-information mirrors written by the firmware into host DRAM
	 * (struct iwx_prph_info). Positive evidence that the firmware is really
	 * executing, as opposed to sitting in ROM. */
	/* RX ring bookkeeping: ALIVE is an FH RX packet. If the hardware index
	 * never moved, the packet was never written. */
	/* xe3 v331: trailing MIC+CRC bytes stripped in iwx_rx_mpdu_mq(). */
	/* Raw CSR snapshots taken from the ALIVE-failure path. */
	/* SecBoot / PC state, only valid when iwx_nic_lock() succeeded. */
	/*
	 * prph_scratch.ctrl_cfg.pnvm_cfg as it stands when the INIT_COMPLETE
	 * wait fails. The firmware learns where the PNVM payload lives only from
	 * these two fields; if pnvm_size is 0 the firmware has nothing to
	 * validate the channel/regulatory tables against, which is the current
	 * best explanation for "answers PNVM_INIT_COMPLETE but never sends
	 * INIT_COMPLETE". Both stay 0 unless iwx_pnvm_handle_section() reached
	 * the point where it writes them.
	 */
	/*
	 * Return value of the PNVM_INIT_COMPLETE wait inside iwx_load_pnvm().
	 *
	 * This is the number that decides whether the whole init sequence gets to
	 * run at all: iwx_load_pnvm() returns it, iwx_load_ucode_wait_alive()
	 * propagates it (:5786), and iwx_run_init_mvm_ucode() bails out at :5820 -
	 * which is BEFORE :5830, so every xd3 field stays at its initialiser and
	 * the boot yields no information about PNVM or INIT at all.
	 *
	 * Sampled right after the wait, before any early return, so it is valid
	 * even when the wait failed. 0x7fffffff = never reached.
	 */
	/*
	 * Which DRAM layout was handed to the firmware, decided by
	 * IWX_UCODE_TLV_CAPA_FRAGMENTED_PNVM_IMG.
	 *
	 * This is the one reading that pnvm_size cannot provide: both layouts
	 * report the same byte count, so a non-zero pnvm_size proves the payload
	 * was published but not that the address points at something the firmware
	 * can dereference. 0 = not reached, 1 = flat, 2 = fragmented.
	 */
	/* Number of payload sections published (0 when not reached). */
	/*
	 * iwx_nvm_get() had three silent failure paths: send_cmd error,
	 * response-length mismatch, and invalid MAC address. All three left
	 * not say which one fired. These four fields make the answer explicit.
	 *
	 *   nvm_err  : the errno iwx_nvm_get() returned (0x7fffffff never ran)
	 *   nvm_alen : actual payload length of the NVM_GET_INFO response
	 *   nvm_xlen : payload length the driver expected
	 *   nvm_v4   : 1 if the driver used the v4 response layout
	 *   nvm_mac  : first three bytes of the MAC it decoded (0xffffff = n/a)
	 */
	/*
	 * Return codes of the three commands that iwx_run_init_mvm_ucode()
	 * issues between ALIVE and the INIT_COMPLETE wait. All three are
	 * failure path the only observable evidence is these numbers. Recorded
	 * verbatim (negative errno), 0x7fffffff = never reached.
	 */
	/*
	 * INIT_COMPLETE observation, split so a single boot answers two
	 * different questions.
	 *
	 * immediately BEFORE iwx_wait_notification() sleeps. IWX_INIT_COMPLETE
	 * (0x01) already set means the notification arrived during the
	 * preceding commands and this wait returns without sleeping - i.e. the
	 * firmware DID send it. Zero means it had not arrived yet, so a
	 * subsequent timeout is a real "firmware never sent it".
	 *
	 * EWOULDBLOCK = 35 = timed out, other = aborted).
	 */
	/*
	 * MSI-X live cause snapshot taken AFTER the INIT_EXTENDED_CFG timeout.
	 * (non-MSI-X) cause register and carries no information once sc_msix=1.
	 * These three are the registers the MSI-X handler actually looks at, so
	 * a non-zero fhc after the timeout means the firmware did raise
	 * something and the vector simply never reached us.
	 */
	/*
	 * CSR_DRAM_INT_TBL_REG as read back after iwx_post_alive().
	 *
	 * Linux never writes this register on MSI-X: iwl_pcie_alloc_ict() is
	 * only called in the non-msix branch (pcie/gen1_2/trans.c:3911-3917),
	 * and iwl_pcie_reset_ict() returns immediately when ict_tbl == NULL
	 * (pcie/gen1_2/rx.c:2218-2219). itlwm allocates ict_dma unconditionally
	 * (ItlIwx.cpp:21195) and iwx_ict_reset() writes the register without a
	 * sc_msix check, so bit 31 (ENABLE) may be telling this device to route
	 * MSI-X causes into the DRAM table instead of CSR_MSIX_*_CAUSES_AD.
	 * Non-zero here with sc_msix == 1 is the smoking gun for fh=hw=0.
	 */
	/*
	 * Content of the ICT DRAM table at the moment INIT_EXTENDED_CFG timed
	 * out. iwx_ict_reset() memsets the table to zero right before it enables
	 * the hardware pointer, so anything non-zero here was written by the
	 * firmware afterwards. This is what separates "the firmware deposited
	 * causes into the DRAM table and the MSI-X handler therefore never saw
	 * them" from "the firmware stayed silent for an unrelated reason" -
	 * without it, a passing ict-gate run cannot distinguish the two.
	 */
	/*
	 * RX hardware index re-read right after the timeout. The only other
	 * sample of it is taken before any command is issued, so without this
	 * "the firmware stopped feeding the ring" is an assumption, not a
	 * measurement.
	 */
	/*
	 * RX accounting for the "scans run but almost nothing is found" stage.
	 *
	 * 43 full scans of 38 channels produce ~1300 RX frames in total, i.e.
	 * roughly one frame per channel dwell, while the same radio under Linux
	 * sees 30+ BSS on every scan. That number alone does not say *where* the
	 * frames are lost, so four counters split it:
	 *
	 *   rx_mpdu      IWX_REPLY_RX_MPDU_CMD records the firmware handed up
	 *   rx_mgmt      management frames that survived iwx_rx_frame()'s
	 *                pre-inputm filtering (AWDL promisc, duplicates, reorder)
	 *   rx_beacon    of those, subtype 8 (beacon)
	 *   rx_proberesp of those, subtype 5 (probe response)
	 *
	 * struct ieee80211com) counts how many of them reached
	 * ieee80211_add_scan(), i.e. actually entered the scan result set.
	 *
	 *   rx_mpdu >> rx_mgmt     -> dropped inside iwx_rx_frame()
	 *   rx_mgmt >> beacons     -> the firmware sends other traffic but no
	 *                             foreign beacons (MAC filter / beacon filter)
	 *   beacons >> beacon_evt  -> net80211 dropped them (badchan / chan
	 *                             mismatch / element parse); ic_stats says which
	 *   beacon_evt >> found    -> the results never left the driver
	 */
	/*
	 * Node table contents at the end of the last scan: every node net80211
	 * holds, and how many of them carry an SSID (i.e. were matched to a real
	 * BSS rather than being an address we only transmitted to). Read at
	 * numbers always describe the same completed scan.
	 */
	/*
	 * Scan bring-up. Observed failure: phrase "no networks listed" while
	 * ic_chan_active is already correct, isDriverAvailable stays 0 and the
	 * IO80211 scan manager never leaves HALTED.
	 *
	 * The chain that produces exactly that is:
	 *   iwx_init() -> ieee80211_begin_scan() -> iwx_newstate() -> newstate_task
	 *   -> iwx_scan() -> iwx_umac_scan() -> iwx_send_cmd(SCAN_REQ_UMAC)
	 * iwx_scan() sets ic->ic_state = S_SCAN itself (ItlIwx.cpp:15027) and only
	 * skips it when iwx_umac_scan() returns an error. iwx_init() then waits up
	 * to 1 s for that state and, on timeout, calls iwx_stop() and returns
	 * EWOULDBLOCK - so IEEE80211_EVT_DRIVER_READY is never fired, the WCL
	 * DriverAvailable message reports "not available" and the scan manager is
	 * pinned in HALTED. Everything downstream (SCAN_REQ refused) follows.
	 *
	 *                      reached S_SCAN, 0x7fffffff = never called)
	 *                      iwx_umac_scan(); negative = returned before send
	 *                      (-ERANGE from the size check, -EIO from
	 *                      iwx_fill_probe_req), 0x7fffffff = never reached
	 */
	/*
	 * in iwx_umac_scan_v17(), sampled right after it returns (-1 = never
	 * latency instead of only reporting the timeout: se=0 with a small ms
	 * means the firmware answered, se=35 with ~1000 ms means it did not.
	 */
	/*
	 * First firmware command that failed, recorded in iwx_send_cmd() - the
	 * single choke point every command goes through. iwx_init_hw() runs a long
	 * list of commands after INIT (tx antenna, BT coex, SOC config, DQA, aux
	 * STA, phy contexts, LTR, temp threshold, power, MCC, SCAN_CFG, beacon
	 * filter) and any one of them timing out makes iwx_init() fail with that
	 * errno - which is exactly what xd9's ie=35 cannot distinguish, because
	 * 35 (EWOULDBLOCK) is both "the 1 s S_SCAN wait expired" and "a command
	 * sleep timed out". The id here names the command, so one boot answers it.
	 *
	 */
	/*
	 * Up to four distinct failing command ids, in arrival order (published
	 * as xd13).
	 *
	 * iwx_init_hw() keeps going after a failed step, so one boot can name
	 * every still-mismatched command instead of one per reboot. The same id
	 * repeats for the three phy contexts, so consecutive duplicates collapse.
	 */
	/*
	 * Commands that the group-0 -> long-group rewrite retargeted from
	 * IWX_LONG_GROUP (0x01) to IWX_ALWAYS_LONG_GROUP (0x0e) because the
	 * sc-a0 CMD_VERSIONS table only lists them in group 14.
	 *
	 * Published through xj "g14=". Zero on AX210/22000 (their tables have
	 * no entry at all, so the historical group-1 path is kept).
	 */
	/*
	 * PHY_CONTEXT_CMD version declared by the firmware's command-version
	 * table (TLV 48), published as xd14 "phyv=".
	 *
	 * The SC image does ship that table (229 entries) and declares version 4
	 * for PHY_CONTEXT_CMD. 3 would mean an older table, 0xff
	 * (IWX_FW_CMD_VER_UNKNOWN) means the table was not parsed at all -- two
	 * very different fixes, and only this field tells them apart. xd14 also
	 * publishes sc->n_cmd_versions ("ncv=") as the direct evidence that the
	 * table was consumed.
	 */
	/*
	 * Scan bring-up chain counters (published through xd11).
	 *
	 * xd9 showed ie=35 with sv/sl/nc still -1, i.e. iwx_init() returned
	 * EWOULDBLOCK without ever reaching iwx_umac_scan(). That leaves the
	 * whole chain
	 *
	 *   iwx_init_hw() -> ieee80211_begin_scan() -> ieee80211_setmode()
	 *   -> ieee80211_next_scan() -> ieee80211_new_state(S_SCAN)
	 *   -> iwx_newstate() -> iwx_newstate_task() -> iwx_scan()
	 *   -> iwx_umac_scan()
	 *
	 * undifferentiated, because 35 is also what every command sleep returns.
	 * Each entry point below bumps one counter, so a single boot says exactly
	 * which link is missing.
	 *
	 *                           (so iwx_init_hw() succeeded)
	 *                           in the command gate
	 *                           (0x7fffffff = iwx_scan() never failed)
	 */
	/*
	 * 1 = iwx_add_aux_sta() returned early for SC.
	 *
	 * SC firmware's command-version table (TLV 48) has 229 entries but does
	 * NOT declare ADD_STA in the LONG group, so iwx_lookup_cmd_ver() answers
	 * UNKNOWN and the original "version >= 12" gate let the legacy ADD_STA
	 * through. SC firmware never acked it (xd10: id=0x118 e=35), stalling
	 * iwx_init_hw() before the scan could start. xd12 records that the new
	 * family gate was taken, so one boot cannot be confused with "ADD_STA
	 * was sent and happened to succeed".
	 */
	/*
	 * MLD (MAC_CONF_GROUP) step errnos, published as xmld
	 * "md=<MAC_CONFIG> lk=<LINK_CONFIG> st=<STA_CONFIG> ky=<SEC_KEY>
	 *  rm=<STA_REMOVE>".
	 *
	 * 0x7fffffff = that step was never issued; 0 = acknowledged; anything
	 * else is the errno. They are the only evidence the MLD branch ran at
	 * all on a boot that never associates, because the six legacy paths that
	 * are now bypassed used to report through xd10.
	 */
	/*
	 * Replies that arrived for a command-queue slot which was still marked
	 * IWX_TXDATA_FLAG_CMD_PENDING, but whose code no case in iwx_rx_pkt()
	 * claimed. That packet falls into the "unhandled firmware response"
	 * default, `handled` becomes 0, iwx_cmd_done() is skipped and the waiter
	 * sleeps out its whole deadline even though the firmware DID answer.
	 *
	 * This is the exact shape of the v333 MAC_CONFIG failure
	 * (xd10="id=0x308 e=35"): five MAC_CONF_GROUP/DATA_PATH case labels had
	 * been dropped when the MLD path was ported, so 0x308/0x309/0x30a/0x30c/
	 * 0x518 were acknowledged by the firmware and thrown away by the driver.
	 * Keeping the counter means the next such omission is visible from a
	 * single boot instead of looking like a dead firmware.
	 *
	 *   resp_unclaimed : how many such replies were discarded
	 *   resp_uc_code   : wide id of the first one (0xffffffff = none yet)
	 */

	struct iwx_dma_info ctxt_info_dma;
	struct iwx_self_init_dram init_dram;
    /* For gen3 */
    struct iwx_dma_info prph_scratch_dma;
    struct iwx_dma_info prph_info_dma;
    struct iwx_dma_info iml_dma;
    /*
     * PNVM payload storage. With IWX_UCODE_TLV_CAPA_FRAGMENTED_PNVM_IMG the
     * payload is split into per-section DRAM blocks (pnvm_seg_dma[0..pnvm_segs-1])
     * and pnvm_dram holds only the address table (struct iwx_pnvm_info_dram).
     * Without that capability pnvm_dram holds the concatenated payload and
     * pnvm_segs stays 0.
     */
    struct iwx_dma_info pnvm_dram;
    struct iwx_dma_info pnvm_seg_dma[IWX_MAX_DRAM_ENTRY];
    size_t pnvm_size;
    int pnvm_segs;

	int sc_fw_chunk_done;
	int sc_init_complete;
#define IWX_INIT_COMPLETE	0x01
#define IWX_CALIB_COMPLETE	0x02
#define IWX_PNVM_COMPLETE    0x04

	struct iwx_ucode_status sc_uc;
	char sc_fwver[32];

	int sc_capaflags;
	int sc_capa_max_probe_len;
	int sc_capa_n_scan_channels;
    int sc_capa_num_stations;
	uint8_t sc_ucode_api[howmany(IWX_NUM_UCODE_TLV_API, NBBY)];
	uint8_t sc_enabled_capa[howmany(IWX_NUM_UCODE_TLV_CAPA, NBBY)];
#define IWX_MAX_FW_CMD_VERSIONS	800
	struct iwx_fw_cmd_version cmd_versions[IWX_MAX_FW_CMD_VERSIONS];
	int n_cmd_versions;
    char sc_fw_mcc[3];
    uint16_t sc_fw_mcc_int;
#define IWX_IML_MAX_LEN 2048*10
    uint32_t sc_iml_len;
    uint8_t sc_iml[IWX_IML_MAX_LEN];

	int sc_intmask;
	int sc_flags;

	uint32_t sc_fh_init_mask;
	uint32_t sc_hw_init_mask;
	uint32_t sc_fh_mask;
	uint32_t sc_hw_mask;

	int sc_generation;

//	struct rwlock ioctl_rwl;

	int sc_cap_off; /* PCIe caps */

	const char *sc_fwname;
	struct iwx_fw_info sc_fw;
	struct iwx_dma_info fw_mon;
	int sc_fw_phy_config;
	struct iwx_tlv_calib_ctrl sc_default_calib[IWX_UCODE_TYPE_MAX];

	struct iwx_nvm_data sc_nvm;
	struct iwx_bf_data sc_bf;

	int sc_tx_timer;
	int sc_rx_ba_sessions;

	int sc_scan_last_antenna;

	int sc_fixed_ridx;
    
    uint8_t sc_mgmt_last_antenna_idx; /* for MGMT frames using*/

	int sc_staid;
	int sc_nodecolor;

	uint8_t *sc_cmd_resp_pkt[IWX_TFD_QUEUE_SIZE_MAX_GEN3];	
	size_t sc_cmd_resp_len[IWX_TFD_QUEUE_SIZE_MAX_GEN3];
	int sc_nic_locks;

	struct taskq *sc_nswq;

	struct iwx_rx_phy_info sc_last_phy_info;
    int sc_ampdu_ref;
#define IWX_MAX_BAID    32
    struct iwx_rxba_data sc_rxba_data[IWX_MAX_BAID];

	uint32_t sc_time_event_uid;

	/* phy contexts.  we only use the first one */
	struct iwx_phy_ctxt sc_phyctxt[IWX_NUM_PHY_CTX];

	int sc_noise;

    int sc_pm_support;
	int sc_ltr_enabled;

	int sc_integrated;
	int sc_tx_with_siso_diversity;
	int sc_max_tfd_queue_size;
    int sc_ltr_delay;
    int sc_xtal_latency;
    int sc_low_latency_xtal;
    
    int sc_uhb_supported;

#if NBPFILTER > 0
	caddr_t			sc_drvbpf;

	union {
		struct iwx_rx_radiotap_header th;
		uint8_t	pad[IEEE80211_RADIOTAP_HDRLEN];
	} sc_rxtapu;
#define sc_rxtap	sc_rxtapu.th
	int			sc_rxtap_len;

	union {
		struct iwx_tx_radiotap_header th;
		uint8_t	pad[IEEE80211_RADIOTAP_HDRLEN];
	} sc_txtapu;
#define sc_txtap	sc_txtapu.th
	int			sc_txtap_len;
#endif
};

struct iwx_node {
	struct ieee80211_node in_ni;
	struct iwx_phy_ctxt *in_phyctxt;
    uint8_t in_macaddr[ETHER_ADDR_LEN];

	uint16_t in_id;
	uint16_t in_color;
    
    struct iwx_rxq_dup_data dup_data;
};
#define IWX_STATION_ID 0
#define IWX_AUX_STA_ID 1
#define IWX_MONITOR_STA_ID 2

#define IWX_ICT_SIZE		4096
#define IWX_ICT_COUNT		(IWX_ICT_SIZE / sizeof (uint32_t))
#define IWX_ICT_PADDR_SHIFT	12
