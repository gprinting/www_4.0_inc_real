<?
include_once(INC_PATH . '/com/nexmotion/job/front/common/OrderCommonDAO.inc');
include_once(INC_PATH . '/com/nexmotion/html/front/order/SheetHTML.inc');

class SheetDAO extends OrderCommonDAO {
    /**
     * @brief 카테고리 낱장여부 검색
     *
     * @param $conn  = connection identifier
     * @param $cate_sortcode = 카테고리 분류코드
     *
     * @return 검색결과
     */
    function selectCateFlattypYn($conn, $cate_sortcode) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query  = "\n SELECT flattyp_yn";
        $query .= "\n   FROM cate";
        $query .= "\n  WHERE sortcode = %s";

        $query  = sprintf($query, $conn->param($cate_sortcode));

        $temp = [];
        $temp[] = $cate_sortcode;

        return $conn->Execute($query, $temp)->fields["flattyp_yn"];
    }

    function selectEarnPoint($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT IFNULL(SUM(earn_point_price),0) as point";
        $query .= "\n FROM order_common";
        $query .= "\n  WHERE member_seqno = %s";//AND order_state >= 2230";

        $query  = sprintf($query, $param["member_seqno"]);
        return $conn->Execute($query)->fields["point"];
    }

    /** 180621 주석처리 -주문서 작성 선입금 못 불러옴-
     * @brief 주문의 카테고리 분류코드 검색
     *
     * @param $conn      = connection identifier
     * @param $seqno_arr = 주문_공통_일련번호 배열
     *
     * @return 쿼리 실행결과
     *
    function selectMemberPrepay($conn, $seqno) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query  = "\n SELECT  A.prepay_price_money";
        $query .= "\n        ,A.prepay_price_card";
        $query .= "\n   FROM  member AS A";
        $query .= "\n  WHERE  A.member_seqno = %s";

        $query  = sprintf($query, $seqno);

        return $conn->Execute($query);
    }*/

    /**
     * @brief 회원 주소 검색
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectMemberDlvr($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $temp = [];
        $temp["col"]  = " dlvr_name";
        $temp["col"] .= ",recei";
        $temp["col"] .= ",tel_num";
        $temp["col"] .= ",cell_num";
        $temp["col"] .= ",zipcode";
        $temp["col"] .= ",addr";
        $temp["col"] .= ",addr_detail";
        $temp["table"] = "member_dlvr";
        $temp["where"]["member_seqno"] = $param["member_seqno"];

        if ($param["basic_yn"] != NULL) {
            $temp["where"]["basic_yn"] = $param["basic_yn"];
        }
        $rs = $this->selectData($conn, $temp);

        return $rs;
    }

    /**
     * @brief 회원 주소 검색
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectMemberDlvrExist($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $temp = [];
        $temp["col"]  = " dlvr_name";
        $temp["col"] .= ",recei";
        $temp["col"] .= ",tel_num";
        $temp["col"] .= ",cell_num";
        $temp["col"] .= ",zipcode";
        $temp["col"] .= ",addr";
        $temp["col"] .= ",addr_detail";
        $temp["table"] = "member_dlvr";
        $temp["where"]["member_seqno"] = $param["member_seqno"];

        if ($param["basic_yn"] != NULL) {
            $temp["where"]["basic_yn"] = $param["basic_yn"];
        }
        if ($param["addr"] != NULL) {
            $temp["where"]["addr"] = $param["addr"];
        }
        if ($param["addr_detail"] != NULL) {
            $temp["where"]["addr_detail"] = $param["addr_detail"];
        }
        $rs = $this->selectData($conn, $temp);

        return $rs;
    }

    /**
     * @brief 회원 주소 검색
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectMembInfo($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query  = "\n SELECT  dlvr_name, recei, tel_num, cell_num, zipcode, addr, addr_detail";
        $query .= "\n   FROM  member_dlvr";
        $query .= "\n  WHERE  member_seqno = %s";
        $query .= "\n    AND  basic_yn = 'Y'";

        $query  = sprintf($query, $conn->param($param["member_seqno"]));

        $temp = [];
        $temp[] = $param["member_seqno"];

        return $conn->Execute($query, $temp);
    }

     /**
     * @brief 회원 주소 검색
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectMembInfo2($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query  = "\n SELECT  member_name, tel_num, cell_num, zipcode, addr, addr_detail";
        $query .= "\n   FROM  member";
        $query .= "\n  WHERE  member_seqno = %s";

        $query  = sprintf($query, $conn->param($param["member_seqno"]));

        $temp = [];
        $temp[] = $param["member_seqno"];

        return $conn->Execute($query, $temp);
    }

    /**
     * @brief 직배 기간인지 확인
     */
    function selectDirectDlvrEndPeriod($conn, $member_seqno) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $member_seqno = $this->parameterEscape($conn, $member_seqno);

        $query  = "\n SELECT  direct_dlvr_yn";
        $query .= "\n   FROM  member";
        $query .= "\n  WHERE  member_seqno = %s";

        $query  = sprintf($query, $member_seqno);

        $rs = $conn->Execute($query);

        return $rs->fields;
    }

	/**
     * @brief 직배사용여부 체크
     */
    function selectDirectDlvrInfo($conn, $member_seqno) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $member_seqno = $this->parameterEscape($conn, $member_seqno);

        $query  = "\n SELECT  direct_dlvr_yn";
        $query .= "\n   FROM  member";
        $query .= "\n  WHERE  member_seqno = %s";

        $query  = sprintf($query, $member_seqno);

        $rs = $conn->Execute($query);

        return $rs->fields;
    }

    /**
     * @brief 배송 유형 체크
     */
    function selectDlvrInfo($conn, $member_seqno) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $member_seqno = $this->parameterEscape($conn, $member_seqno);

        $query  = "\n SELECT  dlvr_add_info";
        $query .= "\n   FROM  member";
        $query .= "\n  WHERE  member_seqno = %s";

        $query  = sprintf($query, $member_seqno);

        $rs = $conn->Execute($query);

        return $rs->fields;
    }

    /**
     * @brief 회원 직배 정보 변경
     */
    function updateMemberDirectDlvr($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query  = "\n UPDATE /* SheetDAO.inc - 회원 직배정보 변경 */";
        $query .= "\n        member";
        $query .= "\n    SET direct_dlvr_yn = %s";
        $query .= "\n  WHERE member_seqno   = %s";

        $query  = sprintf($query, "'N'"
                                , $param);

        return $conn->Execute($query);
    }


    /**
     * @brief 회사 주소 검색
     *
     * @param $conn  = connection identifier
     * @param $sell_site = 판매채널 일련번호
     *
     * @return 검색결과
     */
    function selectCpnAdminInfo($conn, $sell_site) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $temp = [];
        $temp["col"]  = " sell_site      AS name";
        $temp["col"] .= ",repre_num      AS tel_num";
        $temp["col"] .= ",repre_cell_num AS cell_num";
        $temp["col"] .= ",zipcode";
        $temp["col"] .= ",addr";
        $temp["col"] .= ",addr_detail";
        $temp["table"] = "cpn_admin";
        $temp["where"]["cpn_admin_seqno"] = $sell_site;

        $rs = $this->selectData($conn, $temp);

        return $rs->fields;
    }

    /**
     * @brief 회원 배송주소 입력
     *
     * @param $conn  = connection identifier
     * @param $param = 입력값 파라미터
     *
     * @return 검색결과
     */
    function insertMemberDlvr($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n INSERT INTO member_dlvr(";
        $query .= "\n     member_seqno,";
        $query .= "\n     dlvr_name,";
        $query .= "\n     recei,";
        $query .= "\n     tel_num,";
        $query .= "\n     cell_num,";
        $query .= "\n     zipcode,";
        $query .= "\n     addr,";
        $query .= "\n     addr_detail,";
        $query .= "\n     regi_date,";
        $query .= "\n     basic_yn";
        $query .= "\n ) VALUES (";
        $query .= "\n     %s,";
        $query .= "\n     %s,";
        $query .= "\n     %s,";
        $query .= "\n     %s,";
        $query .= "\n     %s,";
        $query .= "\n     %s,";
        $query .= "\n     %s,";
        $query .= "\n     %s,";
        $query .= "\n     now(),";
        $query .= "\n     %s";
        $query .= "\n )";

        $query  = sprintf($query, $param["member_seqno"]
                                , $param["dlvr_name"]
                                , $param["recei"]
                                , $param["tel_num"]
                                , $param["cell_num"]
                                , $param["zipcode"]
                                , $param["addr"]
                                , $param["addr_detail"]
                                , $param["basic_yn"]);

        return $conn->Execute($query);
    }

    /**
     * @brief 주문_파일 row 수 검색
     *
     * @param $conn = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return row 수
     */
    function selectOrderFileCount($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT COUNT(1) AS cnt";
        $query .= "\n   FROM order_file AS A";
        $query .= "\n  WHERE  A.order_file_seqno   = %s";
        $query .= "\n    AND  A.order_common_seqno = %s";
        $query .= "\n    AND  A.member_seqno       = %s";

        $query  = sprintf($query, $param["file_seqno"]
                                , $param["order_seqno"]
                                , $param["member_seqno"]);

        $rs = $conn->Execute($query);

        return $rs->fields["cnt"];
    }

    /**
     * @brief 주문에 묶인 작업파일 검색
     *
     * @param $conn = connection identifier
     * @param $param = 검색조건
     *
     * @return row 수
     */
    function selectOrderFileList($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $except_arr = array("order_common_seqno" => true);

        $param = $this->parameterArrayEscape($conn, $param, $except_arr);

        $query  = "\n SELECT  A.order_file_seqno";
        $query .= "\n        ,A.file_path";
        $query .= "\n        ,A.save_file_name";
        $query .= "\n        ,A.origin_file_name";
        $query .= "\n        ,A.size";
        $query .= "\n   FROM  order_file AS A";
        $query .= "\n  WHERE  A.order_common_seqno IN (%s)";
        $query .= "\n    AND  A.dvs = %s";

        $query  = sprintf($query, $param["order_common_seqno"]
                                , $param["dvs"]);

        $rs = $conn->Execute($query);

        return $rs;
    }

    /**
     * @brief 주문_파일 정보 입력
     *
     * @param $conn = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function insertOrderFile($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $temp = [];
        $temp["table"]                     = "order_file";
        $temp["col"]["dvs"]                = $param["dvs"];
        $temp["col"]["file_path"]          = $param["file_path"];
        $temp["col"]["save_file_name"]     = $param["save_file_name"];
        $temp["col"]["origin_file_name"]   = $param["origin_file_name"];
        $temp["col"]["size"]               = $param["size"];
        $temp["col"]["member_seqno"]       = $param["member_seqno"];
        if ($this->blankParameterCheck($param, "order_common_seqno")) {
            $temp["col"]["order_common_seqno"] = $param["order_common_seqno"];
        }

        return $this->insertData($conn, $temp);
    }

    function insertEstiFile($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $temp = [];
        $temp["table"]                     = "esti_file";
        $temp["col"]["file_path"]          = $param["file_path"];
        $temp["col"]["save_file_name"]     = $param["save_file_name"];
        $temp["col"]["origin_file_name"]   = $param["origin_file_name"];
        $temp["col"]["size"]               = $param["size"];
        $temp["col"]["member_seqno"]       = $param["member_seqno"];
        if ($this->blankParameterCheck($param, "order_common_seqno")) {
            $temp["col"]["order_common_seqno"] = $param["order_common_seqno"];
        }

        return $this->insertData($conn, $temp);
    }

    function insertOtoFile($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $temp = [];
        $temp["table"]                     = "oto_inq_file";
        $temp["col"]["file_path"]          = $param["file_path"];
        $temp["col"]["save_file_name"]     = $param["save_file_name"];
        $temp["col"]["origin_file_name"]   = $param["origin_file_name"];
        $temp["col"]["size"]               = $param["size"];
        $temp["col"]["oto_inq_seqno"]       = $param["oto_inq_seqno"];

        return $this->insertData($conn, $temp);
    }

    function updateOrderFile2($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $temp = [];
        $temp["table"]                     = "order_file";
        $temp["col"]["file_path"]          = $param["file_path"];
        $temp["col"]["save_file_name"]     = $param["save_file_name"];
        $temp["col"]["origin_file_name"]   = $param["origin_file_name"];
        $temp["col"]["size"]               = $param["size"];
        $temp["prk"] = "order_common_seqno";
        $temp["prkVal"] = $param["file_seqno"];

        return $this->updateData($conn, $temp);
    }

    function updateOrderState($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query  = "\n UPDATE  order_common";
        $query .= "\n    SET  order_state = 1320 ";
        $query .= "\n  WHERE  order_state = 1325 ";
        $query .= "\n  AND order_common_seqno = %s ";

        $query  = sprintf($query, $param["file_seqno"]);

        return $conn->Execute($query);
    }

    /**
     * @brief 주문_파일 정보 수정
     *
     * @param $conn = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function updateOrderFile($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query  = "\n UPDATE  order_file";
        $query .= "\n    SET  save_file_name = %s";
        $query .= "\n        ,origin_file_name = %s";
        $query .= "\n        ,size = %s";
        $query .= "\n  WHERE  order_file_seqno = %s";

        $query  = sprintf($query, $conn->param($param["save_file_name"])
                                , $conn->param($param["origin_file_name"])
                                , $conn->param($param["size"])
                                , $conn->param($param["order_file_seqno"]));

        $temp = [];
        $temp[] = $param["save_file_name"];
        $temp[] = $param["origin_file_name"];
        $temp[] = $param["size"];
        $temp[] = $param["order_file_seqno"];

        return $conn->Execute($query, $temp);
    }

    /**
     * @brief 주문의 카테고리 분류코드 검색
     *
     * @param $conn      = connection identifier
     * @param $seqno_arr = 주문_공통_일련번호 배열
     *
     * @return 쿼리 실행결과
     */
    function selectOrderCateSortcode($conn, $seqno_arr) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $seqno = $this->arr2paramStr($conn, $seqno_arr);

        $query  = "\n SELECT  A.cate_sortcode";
        $query .= "\n   FROM  order_common AS A";
        $query .= "\n  WHERE  A.order_common_seqno IN (%s)";

        $query  = sprintf($query, $seqno);

        return $conn->Execute($query);
    }

    /**
     * @brief 사용 가능한 쿠폰 일련번호 검색
     *
     * @param $conn = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 쿼리 실행결과
     */
    function selectValidCpSeqno($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT  A.coupon_seqno";
        $query .= "\n   FROM  coupon_release_history  AS A";
        $query .= "\n   INNER JOIN  coupon  AS B ON A.coupon_seqno = B.coupon_seqno";
        $query .= "\n  WHERE  A.member_seqno = %s AND A.is_used = 'N' ";

        $query  = sprintf($query, $param["member_seqno"]);

        return $conn->Execute($query);
    }

    /**
     * @brief 쿠폰 정보 검색
     *
     * @param $conn = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 쿼리 실행결과
     */
    function selectValidCpInfo($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $except_arr = array("coupon_seqno" => true);

        $param = $this->parameterArrayEscape($conn, $param, $except_arr);

        $query  = "\n SELECT  coupon_seqno";
        $query .= "\n        ,name";
        $query .= "\n        ,release_date";
        $query .= "\n        ,expired_date";
        $query .= "\n        ,detail";
        $query .= "\n        ,discount_rate";
        $query .= "\n        ,categories";
        $query .= "\n   FROM  coupon AS A";
        $query .= "\n  WHERE  A.coupon_seqno IN (%s)";

        $query  = sprintf($query, $param["coupon_seqno"]);
        return $conn->Execute($query);
    }

    /**
     * @brief 택배비 조회시 도서지역인지 확인
     *
     * @param $conn = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 쿼리 실행결과
     */
    function selectParcelCostPrice($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $zipcode = $param['zipcode'];

        $query  = "\n SELECT  price";
        $query .= "\n   FROM  cjparcel_islands AS A";
        $query .= "\n  WHERE  A.zipcode = %s";

        $query  = sprintf($query, $param['zipcode']);

        return $conn->Execute($query);
    }

    /**
     * @brief 택배비 조회시 도서지역인지 확인
     *
     * @param $conn = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 쿼리 실행결과
     */
    function selectAutobikeCostPrice($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $zipcode = $param['zipcode'];

        $query  = "\n SELECT  autobike, damas, rabo, 1ton";
        $query .= "\n   FROM  autobike_dlvr_cost ";
        $query .= "\n  WHERE  new_zipcode = " . $zipcode;
        $query .= "\n ORDER BY autobike DESC ";

        return $conn->Execute($query);
    }

    /**
     * @brief 선택 된 주문 정보 확인
     *
     * @param $conn = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 쿼리 실행결과
     */
    function selectProductList($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query  = "\n SELECT  A.order_detail";
        $query .= "\n        ,A.title";
        $query .= "\n        ,A.order_common_seqno";
        $query .= "\n        ,B.amt   AS s_amt";
        $query .= "\n        ,B.count AS s_count";
        $query .= "\n        ,C.amt   AS b_amt";
        $query .= "\n   FROM order_common AS A";
        $query .= "\n   LEFT OUTER JOIN order_detail AS B";
        $query .= "\n     ON A.order_common_seqno = B.order_common_seqno";
        $query .= "\n   LEFT OUTER JOIN order_detail_brochure AS C";
        $query .= "\n     ON A.order_common_seqno = C.order_common_seqno";
        $query .= "\n    AND C.typ = '표지'";
        $query .= "\n  WHERE  A.order_common_seqno IN (%s)";

        $query  = sprintf($query, $param['order_common_seqno']);

        return $conn->Execute($query);
    }

    /**
     * @brief 주문상세정보 존재하는지 확인
     *
     * @param $conn  = connection identifier
     * @param $param = 주문공통일련번호, 회원일련번호
     * @param $col   = 검색할 필드명
     *
     * @return 존재하면 true, 없으면 false
     */
    function selectOrderDetail($conn, $param, $col, $is_sheet = true) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT  %s";
        $query .= "\n   FROM  order_detail%s";
        $query .= "\n  WHERE  order_common_seqno = %s";

        $query  = sprintf($query, $col
                                , $is_sheet ? '' : "_borochure"
                                , $param["order_common_seqno"]);

        $rs = $conn->Execute($query);

        return $rs;
    }

    /**
     * @brief 주문옵션에 당일판 들어있는지 확인
     *
     * @param $conn  = connection identifier
     * @param $seqno = 주문 공통 일련번호
     *
     * @return 검색결과값
     */
    function selectOrderOptEmergency($conn, $seqno) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query  = "\n SELECT  /* SheetDAO.inc - 주문옵션 당일판 정보 검색 */";
        $query .= "\n         order_opt_history_seqno";
        $query .= "\n        ,depth1";
        $query .= "\n   FROM  order_opt_history AS A";
        $query .= "\n  WHERE  A.order_common_seqno = %s";
        $query .= "\n    AND  A.opt_name = '당일판'";

        $query  = sprintf($query, $conn->param($seqno));

        return $conn->Execute($query, [$seqno])->fields;
    }

    /**
     * @brief 주문 옵션값 삭제
     *
     * @param $conn  = connection identifier
     * @param $seqno = 주문 공통 일련번호
     *
     * @return 검색결과값
     */
    function deleteOrderOptHistory($conn, $seqno) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query  = "\n DELETE /* SheetDAO.inc - 주문옵션내역 삭제 */";
        $query .= "\n   FROM order_opt_history";
        $query .= "\n  WHERE order_opt_history_seqno = %s";

        $query  = sprintf($query, $conn->param($seqno));

        return $conn->Execute($query, [$seqno]);
    }

    function updateOrderExpecRelease($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query  = "\n UPDATE /* SheetDAO.inc - 주문 예상출고일 재계산 */";
        $query .= "\n        order_common";
        $query .= "\n    SET expec_release_date = %s";
        $query .= "\n  WHERE order_common_seqno = %s";

        $query  = sprintf($query, $conn->param($param["expec_release_date"])
                                , $conn->param($param["order_common_seqno"]));

        return $conn->Execute($query, [$param["expec_release_date"],
                                       $param["order_common_seqno"]]);
    }
}
?>
