<?php
/*
 * Copyright (c) 2017 Nexmotion, Inc.
 * All rights reserved.
 *
 * REVISION HISTORY (reverse chronological order)
 *============================================================================
 * 2017-12-05 엄준현 생성
 *============================================================================
 *
 */
include_once(INC_PATH . '/common_dao/ProductCommonDAO.inc');

class AoAdchunhaDAO extends ProductCommonDAO {
    /**
     * @brief 실사제품 주문정보 검색
     *
     * @param $conn  = db 커넥션
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과값
     */
    public function selectAoOrderInfo($conn, $param) {
        if (!$this->connectionCheck($conn)) {
            return false;
        }

        $query  = "\n SELECT  /* [AoAdchunhaDAO.inc] 광고천하api - 실사주문 검색 */";
        $query .= "\n         A.order_common_seqno";
        $query .= "\n        ,A.title";
        $query .= "\n        ,IF(A.amt < 1, A.amt, TRIM(TRAILING '.0' FROM CAST(A.amt AS char))) AS amt";
        $query .= "\n        ,A.cate_sortcode";
        $query .= "\n        ,B.order_detail_seqno";
        $query .= "\n        ,B.cut_size_wid";
        $query .= "\n        ,B.cut_size_vert";
        $query .= "\n        ,B.cate_paper_mpcode";
        $query .= "\n        ,B.cate_beforeside_print_mpcode";
        $query .= "\n   FROM  order_common AS A";
        $query .= "\n        ,order_detail AS B";
        $query .= "\n  WHERE  A.order_common_seqno = B.order_common_seqno";
        $query .= "\n    AND  A.order_num = %s";

        $query  = sprintf($query, $conn->param($param["order_num"]));

        $temp = [];
        $temp[] = $param["order_num"];

        $rs = $conn->Execute($query, $temp);

        return $rs->fields;
    }

    /**
     * @brief 실사제품 주문정보 검색
     *
     * @param $conn  = db 커넥션
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과값
     */
    public function selectAoOrderFileInfo($conn, $param) {
        if (!$this->connectionCheck($conn)) {
            return false;
        }

        $query  = "\n SELECT  /* [AoAdchunhaDAO.inc] 광고천하api - 실사주문 접수파일 검색 */";
        $query .= "\n         A.order_detail_count_file_seqno";
        $query .= "\n        ,A.file_path";
        $query .= "\n        ,A.save_file_name";
        $query .= "\n        ,A.origin_file_name";
        $query .= "\n        ,A.size";
        $query .= "\n   FROM  order_detail_count_file AS A";
        $query .= "\n  WHERE  1 = 1";
        if ($this->blankParameterCheck($param, "order_detail_seqno")) {
            $query .= "\n    AND  A.order_detail_seqno = %s";

            $query  =
                sprintf($query,
                        $conn->param($param["order_detail_seqno"]));
        }
        if ($this->blankParameterCheck($param, "order_detail_count_file_seqno")) {
            $query .= "\n    AND  A.order_detail_count_file_seqno = %s";

            $query  =
                sprintf($query,
                        $conn->param($param["order_detail_count_file_seqno"]));
        }


        $temp = [];
        $temp[] = $param["seqno"];

        $rs = $conn->Execute($query, $temp);

        $rs = $conn->Execute($query, $temp);

        return $rs->fields;
    }

    /**
     * @brief 실사제품 주문 후공정 정보 검색
     *
     * @param $conn  = db 커넥션
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과값
     */
    public function selectAoAfterInfo($conn, $param) {
        if (!$this->connectionCheck($conn)) {
            return false;
        }

        $query  = "\n SELECT  /* [AoAdchunhaDAO.inc] 광고천하api - 실사주문 후공정 내역 검색 */";
        $query .= "\n         A.after_name";
        $query .= "\n        ,A.depth1";
        $query .= "\n        ,A.depth2";
        $query .= "\n        ,A.depth3";
        $query .= "\n        ,A.detail";
        $query .= "\n        ,A.basic_yn";
        $query .= "\n   FROM  order_after_history AS A";
        $query .= "\n  WHERE  A.order_common_seqno = %s";

        $query  = sprintf($query, $conn->param($param["order_common_seqno"]));

        $temp = [];
        $temp[] = $param["order_common_seqno"];

        $rs = $conn->Execute($query, $temp);

        return $rs;
    }

    /**
     * @brief 실사제품 주문 옵션 정보 검색
     *
     * @param $conn  = db 커넥션
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과값
     */
    public function selectAoOptInfo($conn, $param) {
        if (!$this->connectionCheck($conn)) {
            return false;
        }

        $query  = "\n SELECT  /* [AoAdchunhaDAO.inc] 광고천하api - 실사주문 옵션 내역 검색 */";
        $query .= "\n         A.opt_name";
        $query .= "\n        ,A.depth1";
        $query .= "\n        ,A.depth2";
        $query .= "\n        ,A.depth3";
        $query .= "\n        ,A.detail";
        $query .= "\n   FROM  order_opt_history AS A";
        $query .= "\n  WHERE  A.order_common_seqno = %s";

        $query  = sprintf($query, $conn->param($param["order_common_seqno"]));

        $temp = [];
        $temp[] = $param["order_common_seqno"];

        $rs = $conn->Execute($query, $temp);

        return $rs;
    }

    /**
     * @brief 광고천하 주문번호와 디프린팅 주문번호 매칭데이터 입력
     *
     * @param $conn  = db 커넥션
     * @param $param = 입력값 파라미터
     *
     * @return 검색결과값
     */
    public function insertOutsourceOrderNumMp($conn, $param) {
        if (!$this->connectionCheck($conn)) {
            return false;
        }

        $temp = [];
        $temp["table"] = "outsource_order_num_mp";
        $temp["col"]["outsource_order_num"]  = $param["outsource_order_num"];
        $temp["col"]["owncompany_order_num"] = $param["owncompany_order_num"];

        return $this->insertData($conn, $temp);
    }
}
