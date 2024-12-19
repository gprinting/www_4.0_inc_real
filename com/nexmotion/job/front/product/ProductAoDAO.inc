<?php
/*
 * Copyright (c) 2017 Nexmotion, Inc.
 * All rights reserved.
 *
 * REVISION HISTORY (reverse chronological order)
 *============================================================================
 * 2017-11-08 엄준현 생성
 *============================================================================
 *
 */
include_once(INC_PATH . '/common_dao/ProductCommonDAO.inc');

class ProductAoDAO extends ProductCommonDAO {

    /**
     * @brief 실사후공정 정보 검색
     *
     * @param $conn  = db 커넥션
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과값
     */
    public function selectCateAoAfter($conn, $param) {
        if (!$this->connectionCheck($conn)) {
            return false;
        }

        $query  = "\n    SELECT  A.after_name";
        $query .= "\n           ,A.depth1";
        $query .= "\n           ,A.depth2";
        $query .= "\n           ,A.depth3";
        $query .= "\n           ,A.unitprice";
        $query .= "\n           ,B.mpcode";

        $query .= "\n      FROM  ao_after AS A";
        $query .= "\n           ,cate_ao_after AS B";

        $query .= "\n     WHERE  A.ao_after_seqno = B.ao_after_seqno";
        $query .= "\n       AND  B.cate_sortcode = %s";
        $query .= "\n       AND  B.basic_yn = %s";
        $query .= "\n       AND  A.after_name = %s";
        $query .= "\n  ORDER BY  B.seq";
        $query .= "\n           ,A.after_name";
        $query .= "\n           ,A.ao_after_seqno";

        $query  = sprintf($query, $conn->param($param["cate_sortcode"])
                                , $conn->param($param["basic_yn"])
                                , $conn->param($param["after_name"]));

        $temp = [];
        $temp[] = $param["cate_sortcode"];
        $temp[] = $param["basic_yn"];
        $temp[] = $param["after_name"];

        $rs = $conn->Execute($query, $temp);

        return $rs;
    }

    /**
     * @brief 실사후공정 단가 검색
     *
     * @param $conn  = db 커넥션
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과값
     */
    public function selectAoAfterUnitprice($conn, $param) {
        if (!$this->connectionCheck($conn)) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n    SELECT  A.unitprice";
        $query .= "\n      FROM  ao_after AS A";
        $query .= "\n     WHERE  A.after_name = %s";
        if ($this->blankParameterCheck($param ,"depth1")) {
            $query .= "\n       AND  A.depth1 = ";
            $query .= $param["depth1"];
        }
        if ($this->blankParameterCheck($param ,"depth2")) {
            $query .= "\n       AND  A.depth2 = ";
            $query .= $param["depth2"];
        }
        if ($this->blankParameterCheck($param ,"dpeth3")) {
            $query .= "\n       AND  A.depth3 = ";
            $query .= $param["depth3"];
        }

        $query  = sprintf($query, $param["after_name"]);

        $rs = $conn->Execute($query);

        return intval($rs->fields["unitprice"]);
    }

    /**
     * @brief 카테고리 실사 후공정 하위 depth 검색
     *
     * @detail $param["cate_sortcode"] = 카테고리 분류코드
     * $param["after_name"] = 후공정명
     * $param["depth1"] = 후공정 depth1
     * $param["depth2"] = 후공정 depth2
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     * @param $flag  = 맵핑코드 검색여부
     *
     * @return option html
     */
    public function selectCateAoAfterLower($conn, $param, $flag = true) {
        if (!$this->connectionCheck($conn)) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "";

        if ($this->blankParameterCheck($param, "after_name")) {
            $query  = "\n SELECT  A.depth1 AS lower_depth";
        }
        if ($this->blankParameterCheck($param, "depth1")) {
            $query  = "\n SELECT  A.depth2 AS lower_depth";
        }
        if ($this->blankParameterCheck($param, "depth2")) {
            $query  = "\n SELECT  A.depth3 AS lower_depth";
        }
        if ($flag === true) {
            $query .= "\n        ,B.mpcode";
        }
        $query .= "\n   FROM  ao_after AS A";
        $query .= "\n        ,cate_ao_after AS B";
        $query .= "\n  WHERE  A.ao_after_seqno = B.ao_after_seqno";
        $query .= "\n    AND  A.after_name = %s";
        $query .= "\n    AND  B.cate_sortcode = %s";
        if ($this->blankParameterCheck($param, "depth1")) {
            $query .= "\n    AND  A.depth1 = " . $param["depth1"];
        }
        if ($this->blankParameterCheck($param, "depth2")) {
            $query .= "\n    AND  A.depth2 = " . $param["depth2"];
        }
        $query .= "\n  ORDER BY B.seq";

        $query  = sprintf($query, $param["after_name"]
                                , $param["cate_sortcode"]);

        $rs = $conn->Execute($query);

        return $rs;
    }

    /**
     * @brief 실사후공정 단가 검색
     *
     * @param $conn  = db 커넥션
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과값
     */
    public function selectAoOptUnitprice($conn, $param) {
        if (!$this->connectionCheck($conn)) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n    SELECT  A.unitprice";
        $query .= "\n      FROM  ao_opt AS A";
        $query .= "\n     WHERE  A.opt_name = %s";
        if ($this->blankParameterCheck($param ,"depth1")) {
            $query .= "\n       AND  A.depth1 = ";
            $query .= $param["depth1"];
        }
        if ($this->blankParameterCheck($param ,"depth2")) {
            $query .= "\n       AND  A.depth2 = ";
            $query .= $param["depth2"];
        }
        if ($this->blankParameterCheck($param ,"depth3")) {
            $query .= "\n       AND  A.depth3 = ";
            $query .= $param["depth3"];
        }

        $query  = sprintf($query, $param["opt_name"]);

        $rs = $conn->Execute($query);

        return intval($rs->fields["unitprice"]);
    }

    /**
     * @brief 카테고리 실사 후공정 하위 depth 검색
     *
     * @detail $param["cate_sortcode"] = 카테고리 분류코드
     * $param["opt_name"] = 옵션 
     * $param["depth1"] = 옵션 depth1
     * $param["depth2"] = 옵션 depth2
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     * @param $flag  = 맵핑코드 검색여부
     *
     * @return option html
     */
    public function selectCateAoOptLower($conn, $param, $flag = true) {
        if (!$this->connectionCheck($conn)) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        if ($this->blankParameterCheck($param, "opt_name")) {
            $query  = "\n SELECT DISTINCT  A.depth1 AS lower_depth";
        }
        if ($this->blankParameterCheck($param, "depth1")) {
            $query  = "\n SELECT DISTINCT  A.depth2 AS lower_depth";
        }
        if ($this->blankParameterCheck($param, "depth2")) {
            $query  = "\n SELECT DISTINCT  A.depth3 AS lower_depth";
        }
        if ($flag === true) {
            $query .= "\n        ,B.mpcode";
        }
        $query .= "\n   FROM  ao_opt AS A";
        $query .= "\n        ,cate_ao_opt AS B";
        $query .= "\n  WHERE  A.ao_opt_seqno = B.ao_opt_seqno";
        $query .= "\n    AND  A.opt_name = %s";
        $query .= "\n    AND  B.cate_sortcode = %s";
        if ($this->blankParameterCheck($param, "depth1")) {
            $query .= "\n    AND  A.depth1 = " . $param["depth1"];
        }
        if ($this->blankParameterCheck($param, "depth2")) {
            $query .= "\n    AND  A.depth2 = " . $param["depth2"];
        }
        $query .= "\n  ORDER BY lower_depth + 0, B.seq";

        $query  = sprintf($query, $param["opt_name"]
                                , $param["cate_sortcode"]);

        $rs = $conn->Execute($query);

        return $rs;
    }

    /**
     * @brief 실사옵션 중 거치대 옵션 검색
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return option html
     */
    public function selectRackOpt($conn, $param) {
        if (!$this->connectionCheck($conn)) {
            return false;
        }

        $query  = "\n    SELECT  A.depth1";
        $query .= "\n           ,A.depth2";
        $query .= "\n           ,A.depth3";
        $query .= "\n           ,A.unitprice";
        $query .= "\n           ,B.mpcode";

        $query .= "\n      FROM  ao_opt AS A";
        $query .= "\n           ,cate_ao_opt AS B";

        $query .= "\n     WHERE  A.ao_opt_seqno = B.ao_opt_seqno";
        $query .= "\n       AND  B.cate_sortcode = %s";
        $query .= "\n       AND  A.opt_name = %s";
        $query .= "\n  ORDER BY  B.seq";

        $query  = sprintf($query, $conn->param($param["cate_sortcode"])
                                , $conn->param($param["opt_name"])
                                );

        $temp = [];
        $temp[] = $param["cate_sortcode"];
        $temp[] = $param["opt_name"];

        $rs = $conn->Execute($query, $temp);

        return $rs;
    } 

    /**
     * @brief 맵핑코드로 실사옵션 정보 검색
     *
     * @param $conn  = db 커넥션
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과값
     */
    public function selectAoOptInfoByMpcode($conn, $param) {
        if (!$this->connectionCheck($conn)) {
            return false;
        }

        $query  = "\n    SELECT  A.opt_name";
        $query .= "\n           ,A.depth1";
        $query .= "\n           ,A.depth2";
        $query .= "\n           ,A.depth3";
        $query .= "\n           ,A.unitprice";
        $query .= "\n      FROM  ao_opt AS A";
        $query .= "\n           ,cate_ao_opt AS B";
        $query .= "\n     WHERE  A.ao_opt_seqno = B.ao_opt_seqno";
        $query .= "\n       AND  B.mpcode = %s";

        $query  = sprintf($query, $conn->param($param["opt_name"]));

        $temp = [];
        $temp[] = $param["mpcode"];

        $rs = $conn->Execute($query, $temp);

        return $rs->fields;
    }

    /**
     * @brief 맵핑코드로 실사후공정 정보 검색
     *
     * @param $conn  = db 커넥션
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과값
     */
    public function selectAoAfterInfoByMpcode($conn, $param) {
        if (!$this->connectionCheck($conn)) {
            return false;
        }

        $query  = "\n    SELECT  A.after_name";
        $query .= "\n           ,A.depth1";
        $query .= "\n           ,A.depth2";
        $query .= "\n           ,A.depth3";
        $query .= "\n           ,A.unitprice";
        $query .= "\n      FROM  ao_after AS A";
        $query .= "\n           ,cate_ao_after AS B";
        $query .= "\n     WHERE  A.ao_after_seqno = B.ao_after_seqno";
        $query .= "\n       AND  B.mpcode = %s";

        $query  = sprintf($query, $conn->param($param["opt_name"]));

        $temp = [];
        $temp[] = $param["mpcode"];

        $rs = $conn->Execute($query, $temp);

        return $rs->fields;
    }
}
