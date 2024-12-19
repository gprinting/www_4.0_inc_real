<?
include_once(INC_PATH . '/com/nexmotion/job/nimda/common/BusinessCommonDAO.inc');
include_once(INC_PATH . '/com/nexmotion/html/nimda/business/order_mng/OrderCommonMngHtml.inc');
include_once(INC_PATH . '/com/nexmotion/html/nimda/business/order_mng/OrderTreeHTML.inc');
include_once(INC_PATH . '/com/nexmotion/doc/nimda/business/order_mng/OrderCommonMngDOC.inc');

/**
 * @file MemberCommonListDAO.inc
 *
 * @brief 영업 - 주문관리 - 주문통합관리 DAO
 */
class OrderCommonMngDAO extends BusinessCommonDAO {

    /**
     * @brief 생성자
     *
     */
    function __construct() {
    }

    /**
     * @brief 주문 리스트 검색조건 없이 검색
     *
     * @detail 대용량 게시판 쿼리용
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectOrderListHtml($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

      
        $start_date  = $param["start_date"];
        $end_date  = $param["end_date"];
        $search_dvs  = $param["search_dvs"];
        $order_state  = $param["order_state"];


        $chk_paper = $param["val_paper"];
        if($param["keyword"] == "" && $search_dvs != "bun_dlvr_order_num") $search_dvs = "";

        $param = $this->parameterArrayEscape($conn, $param);
        $dvs = substr($param["dvs"], 1, -1);
        if ($dvs == "COUNT") {
            $query = "\n SELECT  COUNT(DISTINCT A.order_common_seqno) AS cnt";
        } else {
            $query = "\n SELECT  A.order_common_seqno AS seqno";
            $query .= "\n        ,C.sell_site";
            $query .= "\n        ,A.order_num";
            $query .= "\n        ,A.page_cnt";
            $query .= "\n        ,A.order_regi_date";
            $query .= "\n        ,A.depo_finish_date";
            $query .= "\n        ,B.office_nick";
            $query .= "\n        ,B.member_name";
            $query .= "\n        ,A.title";
            $query .= "\n        ,A.amt";
            $query .= "\n        ,A.amt_unit_dvs";
            $query .= "\n        ,A.count";
            $query .= "\n        ,A.order_mng";
            $query .= "\n        ,A.order_detail";
            $query .= "\n        ,A.file_upload_dvs";
            $query .= "\n        ,F.cut_size_wid";
            $query .= "\n        ,F.cut_size_vert";
            $query .= "\n        ,F.work_size_wid";
            $query .= "\n        ,F.work_size_vert";
            $query .= "\n        ,F.receipt_size_wid";
            $query .= "\n        ,F.receipt_size_vert";
            $query .= "\n        ,D.erp_state_name";
            $query .= "\n        ,D.state_code";
            $query .= "\n        ,A.pay_way";
            $query .= "\n        ,A.dlvr_dvs";
            $query .= "\n        ,A.oper_sys";
            $query .= "\n        ,A.sell_price";
            $query .= "\n        ,A.pay_price";
            $query .= "\n        ,A.add_after_price";
            $query .= "\n        ,A.add_opt_price";
            $query .= "\n        ,A.cate_sortcode";
            $query .= "\n        ,A.OPI_Date";
            $query .= "\n        ,B.member_seqno";
            $query .= "\n        ,E.file_path";
            $query .= "\n        ,E.save_file_name";
            $query .= "\n        ,E.origin_file_name";
            $query .= "\n        ,E.size";
            $query .= "\n        ,E.upload_time";
            $query .= "\n        ,E.accept_file_path";
            $query .= "\n        ,E.accept_file_name";
            $query .= "\n        ,E.preview_file_path";
            $query .= "\n        ,E.preview_file_name";
            $query .= "\n        ,A.cust_memo";
            $query .= "\n        ,A.work_memo";
            $query .= "\n        ,A.accept_memo";
            $query .= "\n        ,A.expec_weight";
            $query .= "\n        ,B.mail";
            $query .= "\n        ,B.tel_num";
            $query .= "\n        ,B.cell_num";
            $query .= "\n        ,B.default_release";
            $query .= "\n        ,L.dlvr_way";
            $query .= "\n        ,L.dlvr_sum_way";
            $query .= "\n        ,Q.state_modi_date";
            $query .= "\n        ,R.opt_name";
            $query .= "\n        ,R.depth1";
            $query .= "\n        ,R.depth2";
            $query .= "\n        ,R.depth3";
            $query .= "\n        ,B.sell_channel";

            if($search_dvs == "typset_num") {
                if ($this->blankParameterCheck($param, "keyword")) {
                    $query .= "\n        ,G.typset_num";
                    $query .= "\n        ,CONCAT(I.file_path, I.save_file_name) as typset_pdf ";
                    $query .= "\n        ,CONCAT(J.file_path, J.save_file_name) as typset_label";
                }
            }
        }

        $query .= "\n   FROM  order_common AS A";
        if($search_dvs == "typset_num") {
            if ($this->blankParameterCheck($param, "keyword")) {
                $query .= "\n   LEFT JOIN order_typset  AS G ON G.order_num = A.order_num ";
                $query .= "\n   LEFT JOIN sheet_typset AS H ON G.typset_num = H.typset_num ";
                $query .= "\n   LEFT JOIN sheet_typset_file AS I ON H.sheet_typset_seqno = I.sheet_typset_seqno ";
                $query .= "\n   LEFT JOIN sheet_typset_label_file AS J ON H.sheet_typset_seqno = J.sheet_typset_seqno ";
            }
        }

        if ($this->blankParameterCheck($param, "after")) {
            $query .= "\n   LEFT JOIN order_after_history AS R ON R.order_common_seqno = A.order_common_seqno ";
        }

        $query .= "\n        INNER JOIN member       AS B ON A.member_seqno = B.member_seqno";
        $query .= "\n        INNER JOIN cpn_admin    AS C ON A.cpn_admin_seqno = C.cpn_admin_seqno";
        $query .= "\n        INNER JOIN state_admin    AS D ON A.order_state = D.state_code";
        $query .= "\n        LEFT JOIN order_file    AS E ON A.order_common_seqno = E.order_common_seqno";
        $query .= "\n        INNER JOIN order_detail    AS F ON A.order_common_seqno = F.order_common_seqno";
        $query .= "\n        LEFT JOIN order_dlvr    AS K ON A.order_common_seqno = K.order_common_seqno AND K.tsrs_dvs = '송신'";
        $query .= "\n        LEFT JOIN order_dlvr    AS L ON A.order_common_seqno = L.order_common_seqno AND L.tsrs_dvs = '수신'";
        $query .= "\n        LEFT JOIN order_state_history AS Q ON A.order_common_seqno = Q.order_common_seqno AND Q.state = '1380'";
        $query .= "\n        LEFT JOIN order_opt_history AS R ON A.order_common_seqno = R.order_common_seqno AND R.opt_name IN ('당일판','빠른출고')";
        if($search_dvs == "order_mng" || $this->blankParameterCheck($param, "is_auto")) {
            $query .= "\n        INNER JOIN empl AS O ON A.order_mng = O.empl_id ";
            $query .= "\n        INNER JOIN empl_human_info AS P ON O.empl_seqno = P.empl_seqno ";
        }

        $query .= "\n  WHERE  1 = 1 ";
        //$query .= "\n    AND  A.cpn_admin_seqno =  %s";
        if($search_dvs != "typset_num" && $search_dvs != "order_num" && $search_dvs != "bun_dlvr_order_num") {
            $query .= "\n    AND  A.depo_finish_date >= '" . $start_date . " 00:00:00'";
            $query .= "\n    AND  A.depo_finish_date <= '" . $end_date . " 23:59:59'";
        }

        if ($this->blankParameterCheck($param, "sortcode")) {
            $sortcode = substr($param["sortcode"], 1, -1);
            if($sortcode == "005001001") // 합판
            {
                $query .= "\n    AND  (A.cate_sortcode = '008001003' OR (A.cate_sortcode LIKE '";
                $query .= substr($param["sortcode"], 1, -1) . "%%'";
                $query .= "\n   AND F.cate_paper_mpcode = 243)) ";
            } else if($sortcode == "005002001") // 독판
            {
                $query .= "\n    AND  (A.cate_sortcode LIKE '005%%' OR A.cate_sortcode LIKE '001%%' OR A.cate_sortcode LIKE '008001005%%' OR A.cate_sortcode LIKE '008001002%%' OR A.cate_sortcode LIKE '008001006%%' OR A.cate_sortcode LIKE '011002001%%' OR A.cate_sortcode LIKE '011008001%%') ";
                $query .= "\n   AND F.cate_paper_mpcode != 243 ";
            } else if(startsWith($sortcode, "003003")) // 카드명함
            {
                if(startsWith($sortcode, "003003002")) {
                    $query .= "\n    AND  (A.cate_sortcode LIKE '003003002') ";
                } else {
                    $query .= "\n    AND  A.cate_sortcode LIKE '003003%%' AND F.cate_paper_mpcode != 198 ";
                }
            } else if($sortcode == "003002999") // VIP
            {
                $query .= "\n    AND  (A.cate_sortcode LIKE '003%%' AND A.order_detail like '%%Extra%%')"; 
            } else if ($sortcode == "004001001") //사각재단스티커 
            {
                $query .= "\n    AND  A.cate_sortcode  = '004001001' ";

                if($chk_paper == 1){
                    $query .= "\n   AND  F.order_detail LIKE '%%유포지%%'";
                }else if($chk_paper == 2){
                    $query .= "\n   AND  F.order_detail LIKE '%%투명데드롱%%' ";
                }else if($chk_paper == 3){
                    $query .= "\n   AND  F.order_detail LIKE '%%은무데드롱%%' ";
                }else if($chk_paper == 4){
                    $query .= "\n   AND  F.order_detail LIKE '%%모조지%%' ";
                }else if($chk_paper == 5){
                    $query .= "\n   AND  F.order_detail LIKE '%%크라프트%%' ";
                }else if($chk_paper == 6){
                    $query .= "\n   AND  F.order_detail LIKE '%%아트지%%' ";
                }
            } else if ($sortcode == "004003009") //자유형도무송 
            {
                $query .= "\n    AND  A.cate_sortcode  = '004003009' ";

                if($chk_paper == 1){
                    $query .= "\n   AND  F.order_detail LIKE '%%유포지%%'";
                }else if($chk_paper == 2){
                    $query .= "\n   AND  F.order_detail LIKE '%%투명데드롱%%' ";
                }else if($chk_paper == 3){
                    $query .= "\n   AND  F.order_detail LIKE '%%은무데드롱%%' ";
                }else if($chk_paper == 4){
                    $query .= "\n   AND  F.order_detail LIKE '%%모조지%%' ";
                }else if($chk_paper == 5){
                    $query .= "\n   AND  F.order_detail LIKE '%%크라프트%%' ";
                }else if($chk_paper == 6){
                    $query .= "\n   AND  F.order_detail LIKE '%%아트지%%' ";
                }
            } else if ($sortcode == "003002001") //명함 유포지 
            {
                $query .= "\n    AND  A.cate_sortcode  = '003002001' ";
                // $query .= "\n   AND F.cate_paper_mpcode != 243 ";

                if($chk_paper == 1){
                    $query .= "\n   AND  F.order_detail LIKE '%%유포지%%' and F.tot_tmpt = '4' " ;
                }else if($chk_paper == 2){
                    $query .= "\n   AND  F.order_detail LIKE '%%유포지%%' and F.tot_tmpt = '8'" ;
                }
            }else if(startsWith($sortcode, "003")) // 명함
            {
                if(startsWith($sortcode, "003008")) {
                    $query .= "\n    AND  (A.cate_sortcode LIKE '003008%%') ";
                } else if(startsWith($sortcode, "003007")) {
                    $query .= "\n    AND  (A.cate_sortcode LIKE '003007%%') ";
                } else if(startsWith($sortcode, "003010")) {
                    $query .= "\n    AND  (A.cate_sortcode LIKE '003010%%') ";
                } else if(startsWith($sortcode, "003011")) {
                    $query .= "\n    AND  (A.cate_sortcode LIKE '003011%%') ";
                } else if(startsWith($sortcode, "003012")) {
                    $query .= "\n    AND  (A.cate_sortcode LIKE '003012%%') ";
                } else if(startsWith($sortcode, "003002002")) {
                    $query .= "\n    AND  (A.cate_sortcode LIKE '003002002') ";
                } else if(startsWith($sortcode, "003002003")) {
                    $query .= "\n    AND  (A.cate_sortcode LIKE '003002003') ";
                } else if(startsWith($sortcode, "003002003")) {
                    $query .= "\n    AND  (A.cate_sortcode LIKE '003002003') ";
                } else if(startsWith($sortcode, "003001")) {
                    $query .= "\n    AND  (A.cate_sortcode LIKE '003001%%') ";
                } else if(startsWith($sortcode, "003002")) {
                    $query .= "\n    AND  (A.cate_sortcode LIKE '003002%%') ";
                } else {
                    $query .= "\n    AND  (A.cate_sortcode LIKE '003%%' ";
                    $query .= "\n    OR A.cate_sortcode LIKE '011001001' ";
                    $query .= "\n    OR A.cate_sortcode LIKE '011003001' ";
                    $query .= "\n    OR A.cate_sortcode LIKE '011003001' ";
                    $query .= "\n    OR A.cate_sortcode LIKE '008001001') ";
                }
            } else if(startsWith($sortcode, "003002001") && $param["order_state"] == "2220") // VIP
            {
                $query .= "\n    AND  (A.cate_sortcode LIKE '003%%' OR A.cate_sortcode = '008001001')";
            } else if(startsWith($sortcode, "008002")) // 자석
            {
                $query .= "\n    AND  (A.cate_sortcode LIKE '008002%%')";
            } else if(startsWith($sortcode, "002001") || startsWith($sortcode, "002002")) // 배너
            {
                $query .= "\n    AND  (A.cate_sortcode LIKE '002001%%' OR A.cate_sortcode LIKE '002002%%')";
            } else if(startsWith($sortcode, "007")) //경인쇄,
            {
                $query .= "\n    AND  (A.cate_sortcode LIKE '007%%' OR A.cate_sortcode LIKE '006002%%')";
            } else if(startsWith($sortcode, "004")) //스티커,
            {
                $query .= "\n    AND  (A.cate_sortcode LIKE '004%%' OR A.cate_sortcode LIKE '011004%%')";
            } else {
                $query .= "\n    AND  A.cate_sortcode LIKE '";
                $query .= substr($param["sortcode"], 1, -1) . "%%'";
            }
        }
        if($search_dvs == "title") {
            if ($this->blankParameterCheck($param, "keyword")) {
                $query .= "\n    AND  A.title LIKE ";
                $query .= "'%%" . substr($param["keyword"], 1, -1) . "%%'";
            }
        }

        if($search_dvs == "order_num") {
            if ($this->blankParameterCheck($param, "keyword")) {
                $query .= "\n    AND  A.order_num = ";
                $query .= "'" . substr($param["keyword"], 1, -1) . "'";
            }
        }

        if($search_dvs == "bun_dlvr_order_num") {
            $query .= "\n    AND  A.member_seqno = " . $param["member_seqno"];
            $query .= "\n    AND  K.bun_dlvr_order_num = " . $param["bun_dlvr_order_num"];
        }

        if ($this->blankParameterCheck($param, "member_name")) {
            $query .= "\n    AND  B.member_name LIKE ";
            $query .= "'%%" . substr($param["member_name"], 1, -1) . "%%'";
        }

        if ($this->blankParameterCheck($param, "version")) {
            $version = substr($param["version"], 1, -1);
            if($version == "3") {
                $query .= "\n    AND  A.OPI_Date IS NOT NULL ";
            } else if($version == "4") {
                $query .= "\n    AND  A.OPI_Date IS NULL ";
            }
        }

        if ($this->blankParameterCheck($param, "title")) {
            $query .= "\n    AND  A.title LIKE ";
            $query .= "'%%" . substr($param["title"], 1, -1) . "%%'";
        }

        if ($this->blankParameterCheck($param, "is_quick")) {
            $query .= "\n    AND  R.opt_name = '당일판' ";
        }

        if ($this->blankParameterCheck($param, "is_quick_today")) {
            $query .= "\n    AND  R.depth1 = '오늘출고' ";
        }

        if ($this->blankParameterCheck($param, "is_quick_tomorrow")) {
            $query .= "\n    AND  R.depth1 = '내일출고' ";
        }

        if ($this->blankParameterCheck($param, "is_auto")) {
            if($param["is_auto"] == "'all'") {
                $query .= "\n    AND  O.empl_id LIKE ";
                $query .= "'auto%%'";
            } else if($param["is_auto"] == "'memo'") {
                $query .= "\n    AND  O.empl_id LIKE ";
                $query .= "'auto%%'";
                $query .= "\n    AND  A.cust_memo != '' ";
            }
        }

        if($search_dvs == "order_mng") {
            if ($this->blankParameterCheck($param, "keyword")) {
                $query .= "\n    AND  P.name LIKE ";
                $query .= "'%%" . substr($param["keyword"], 1, -1) . "%%'";
            }
        }

        if($search_dvs == "invo_num") {
            if ($this->blankParameterCheck($param, "keyword")) {
                $query .= "\n    AND  L.invo_num LIKE ";
                $query .= "'%%" . substr($param["keyword"], 1, -1) . "%%'";
            }
        }

        if($search_dvs == "typset_num") {
            if ($this->blankParameterCheck($param, "keyword")) {
                $query .= "\n    AND  G.typset_num = ";
                $query .= "'" . substr($param["keyword"], 1, -1) . "'";
            }
        }

        if ($this->blankParameterCheck($param, "dlvr_way")) {
            $query .= "\n    AND  L.dlvr_way = " . $param["dlvr_way"];
        }

        if ($this->blankParameterCheck($param, "after")) {
            if($param["after"] == "'Y'")
                $query .= "\n    AND  R.after_name IS NOT NULL AND R.basic_yn = 'N' ";
            else if($param["after"] == "'N'")
                $query .= "\n    AND  R.after_name IS NULL AND R.basic_yn = 'N' ";
            else
                $query .= "\n    AND  R.after_name = " . $param["after"];
        }

        if($order_state != "") {
            if ($this->blankParameterCheck($param, "order_state")) {
                if($order_state == "1320") {
                    $query .= "\n    AND  (A.order_state = " . $param["order_state"] . " OR (A.file_upload_dvs = 'R' AND A.order_state != '1180') OR A.order_state = '1385' OR ((A.OPI_Inserted = 'Y' OR A.OPI_Date IS NULL) AND A.order_state = '1380' AND A.receipt_finish_date < DATE_ADD(NOW(), INTERVAL -10 MINUTE)) ";

                    if($search_dvs == "order_mng") {
                        if ($this->blankParameterCheck($param, "keyword")) {
                            $query .= "OR A.order_state = '1380' OR A.order_state = '2120') ";
                        }
                    } else {
                        $query .= ")";
                    }

                    $query .= "\n    AND  A.file_upload_dvs IN ('R','Y') ";
                } else {
                    $query .= "\n    AND  A.order_state = " . $param["order_state"];
                }
            }
        } else {
            $query .= "\n    AND  A.order_state NOT IN ('1120', '1220')";
        }

        //// 추후 제거
        if($param["empl_id"] == "'dpuser1'") {
            //$query .= "\n    AND  B.member_seqno = 6178 ";
        } else {
            $query .= "\n    AND  B.member_seqno != 6178 ";
        }
        //// 추후 제거
        ///
        if($dvs != "COUNT") {
            $query .= "\n  GROUP BY A.order_common_seqno";
            if($param["order_state"] == "'1320'") {
                $query .= " ORDER BY A.file_upload_dvs ASC, R.opt_name DESC ,A.order_common_seqno ASC";
            } else if($param["order_state"] == "'1380'") {
                $query .= " ORDER BY R.opt_name DESC, A.receipt_finish_date ASC";
            } else {
                $query .= " ORDER BY R.opt_name DESC, A.order_common_seqno DESC";
            }
            $query .= "\n  LIMIT %s, %s";
        }

        /*
        $query  = sprintf($query, $param["sell_site"]
                                , substr($param["limit_block"], 1, -1)
                                , substr($param["list_size"], 1, -1));
        */
        $query  = sprintf($query
                                , substr($param["limit_block"], 1, -1)
                                , substr($param["list_size"], 1, -1));
        //echo $query;
        //$conn->debug = 1;
        $rs = $conn->Execute($query);
        if($dvs == "COUNT") {
            return $rs;
        }
        else
            return makeOrderListHtml($conn, $this, $rs, $param);
    }

     /**
     * @brief 주문 리스트 검색조건 없이 검색
     *
     * @detail 대용량 게시판 쿼리용
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectOrderListHtml2($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

      
        $start_date  = $param["start_date"];
        $end_date  = $param["end_date"];
        $search_dvs  = $param["search_dvs"];
        $order_state  = $param["order_state"];


        $chk_paper = $param["val_paper"];
        if($param["keyword"] == "" && $search_dvs != "bun_dlvr_order_num") $search_dvs = "";

        $param = $this->parameterArrayEscape($conn, $param);
        $dvs = substr($param["dvs"], 1, -1);
        if ($dvs == "COUNT") {
            $query = "\n SELECT count(member_pay_history_seqno) as cnt ,SUM(pay_price) AS amt , SUM(card_pay_price) AS amt2 FROM ( SELECT S.* ";
        } else {
            $query = "\n SELECT  A.order_common_seqno AS seqno";
            $query .= "\n        ,C.sell_site";
            $query .= "\n        ,A.order_num";
            $query .= "\n        ,A.page_cnt";
            $query .= "\n        ,A.order_regi_date";
            $query .= "\n        ,A.depo_finish_date";
            $query .= "\n        ,B.office_nick";
            $query .= "\n        ,B.member_name";
            $query .= "\n        ,A.title";
            $query .= "\n        ,A.amt";
            $query .= "\n        ,A.amt_unit_dvs";
            $query .= "\n        ,A.count";
            $query .= "\n        ,A.order_mng";
            $query .= "\n        ,A.order_detail";
            $query .= "\n        ,A.file_upload_dvs";
            $query .= "\n        ,F.cut_size_wid";
            $query .= "\n        ,F.cut_size_vert";
            $query .= "\n        ,F.work_size_wid";
            $query .= "\n        ,F.work_size_vert";
            $query .= "\n        ,F.receipt_size_wid";
            $query .= "\n        ,F.receipt_size_vert";
            $query .= "\n        ,D.erp_state_name";
            $query .= "\n        ,D.state_code";
            $query .= "\n        ,A.pay_way";
            $query .= "\n        ,A.dlvr_dvs";
            $query .= "\n        ,A.oper_sys";
            $query .= "\n        ,A.sell_price";
            $query .= "\n        ,A.pay_price";
            $query .= "\n        ,A.add_after_price";
            $query .= "\n        ,A.add_opt_price";
            $query .= "\n        ,A.cate_sortcode";
            $query .= "\n        ,A.OPI_Date";
            $query .= "\n        ,B.member_seqno";
            $query .= "\n        ,E.file_path";
            $query .= "\n        ,E.save_file_name";
            $query .= "\n        ,E.origin_file_name";
            $query .= "\n        ,E.size";
            $query .= "\n        ,E.upload_time";
            $query .= "\n        ,E.accept_file_path";
            $query .= "\n        ,E.accept_file_name";
            $query .= "\n        ,E.preview_file_path";
            $query .= "\n        ,E.preview_file_name";
            $query .= "\n        ,A.cust_memo";
            $query .= "\n        ,A.work_memo";
            $query .= "\n        ,A.accept_memo";
            $query .= "\n        ,A.expec_weight";
            $query .= "\n        ,B.mail";
            $query .= "\n        ,B.tel_num";
            $query .= "\n        ,B.cell_num";
            $query .= "\n        ,B.default_release";
            $query .= "\n        ,L.dlvr_way";
            $query .= "\n        ,L.dlvr_sum_way";
            $query .= "\n        ,Q.state_modi_date";
            $query .= "\n        ,R.opt_name";
            $query .= "\n        ,R.depth1";
            $query .= "\n        ,R.depth2";
            $query .= "\n        ,R.depth3";
            $query .= "\n        ,B.sell_channel";

            if($search_dvs == "typset_num") {
                if ($this->blankParameterCheck($param, "keyword")) {
                    $query .= "\n        ,G.typset_num";
                    $query .= "\n        ,CONCAT(I.file_path, I.save_file_name) as typset_pdf ";
                    $query .= "\n        ,CONCAT(J.file_path, J.save_file_name) as typset_label";
                }
            }
        }

        $query .= "\n   FROM  order_common AS A";
        if($search_dvs == "typset_num") {
            if ($this->blankParameterCheck($param, "keyword")) {
                $query .= "\n   LEFT JOIN order_typset  AS G ON G.order_num = A.order_num ";
                $query .= "\n   LEFT JOIN sheet_typset AS H ON G.typset_num = H.typset_num ";
                $query .= "\n   LEFT JOIN sheet_typset_file AS I ON H.sheet_typset_seqno = I.sheet_typset_seqno ";
                $query .= "\n   LEFT JOIN sheet_typset_label_file AS J ON H.sheet_typset_seqno = J.sheet_typset_seqno ";
            }
        }

        if ($this->blankParameterCheck($param, "after")) {
            $query .= "\n   LEFT JOIN order_after_history AS R ON R.order_common_seqno = A.order_common_seqno ";
        }

        $query .= "\n        INNER JOIN member       AS B ON A.member_seqno = B.member_seqno";
        $query .= "\n        INNER JOIN cpn_admin    AS C ON A.cpn_admin_seqno = C.cpn_admin_seqno";
        $query .= "\n        INNER JOIN state_admin    AS D ON A.order_state = D.state_code";
        $query .= "\n        LEFT JOIN order_file    AS E ON A.order_common_seqno = E.order_common_seqno";
        $query .= "\n        INNER JOIN order_detail    AS F ON A.order_common_seqno = F.order_common_seqno";
        $query .= "\n        LEFT JOIN order_dlvr    AS K ON A.order_common_seqno = K.order_common_seqno AND K.tsrs_dvs = '송신'";
        $query .= "\n        LEFT JOIN order_dlvr    AS L ON A.order_common_seqno = L.order_common_seqno AND L.tsrs_dvs = '수신'";
        $query .= "\n        LEFT JOIN order_state_history AS Q ON A.order_common_seqno = Q.order_common_seqno AND Q.state = '1380'";
        $query .= "\n        LEFT JOIN order_opt_history AS R ON A.order_common_seqno = R.order_common_seqno AND R.opt_name IN ('당일판','빠른출고')";
        $query .= "\n        LEFT JOIN member_pay_history AS S ON A.order_num = S.order_num AND S.dvs = '매출증가'";
        if($search_dvs == "order_mng" || $this->blankParameterCheck($param, "is_auto")) {
            $query .= "\n        INNER JOIN empl AS O ON A.order_mng = O.empl_id ";
            $query .= "\n        INNER JOIN empl_human_info AS P ON O.empl_seqno = P.empl_seqno ";
        }

        $query .= "\n  WHERE  1 = 1 ";
        //$query .= "\n    AND  A.cpn_admin_seqno =  %s";
        if($search_dvs != "typset_num" && $search_dvs != "order_num" && $search_dvs != "bun_dlvr_order_num") {
            $query .= "\n    AND  A.depo_finish_date >= '" . $start_date . " 00:00:00'";
            $query .= "\n    AND  A.depo_finish_date <= '" . $end_date . " 23:59:59'";
        }

        if ($this->blankParameterCheck($param, "sortcode")) {
            $sortcode = substr($param["sortcode"], 1, -1);
            if($sortcode == "005001001") // 합판
            {
                $query .= "\n    AND  (A.cate_sortcode = '008001003' OR (A.cate_sortcode LIKE '";
                $query .= substr($param["sortcode"], 1, -1) . "%%'";
                $query .= "\n   AND F.cate_paper_mpcode = 243)) ";
            } else if($sortcode == "005002001") // 독판
            {
                $query .= "\n    AND  (A.cate_sortcode LIKE '005%%' OR A.cate_sortcode LIKE '001%%' OR A.cate_sortcode LIKE '008001005%%' OR A.cate_sortcode LIKE '008001002%%' OR A.cate_sortcode LIKE '008001006%%' OR A.cate_sortcode LIKE '011002001%%' OR A.cate_sortcode LIKE '011008001%%') ";
                $query .= "\n   AND F.cate_paper_mpcode != 243 ";
            } else if(startsWith($sortcode, "003003")) // 카드명함
            {
                if(startsWith($sortcode, "003003002")) {
                    $query .= "\n    AND  (A.cate_sortcode LIKE '003003002') ";
                } else {
                    $query .= "\n    AND  A.cate_sortcode LIKE '003003%%' AND F.cate_paper_mpcode != 198 ";
                }
            } else if($sortcode == "003002999") // VIP
            {
                $query .= "\n    AND  (A.cate_sortcode LIKE '003%%' AND A.order_detail like '%%Extra%%')"; 
            } else if ($sortcode == "004001001") //사각재단스티커 
            {
                $query .= "\n    AND  A.cate_sortcode  = '004001001' ";

                if($chk_paper == 1){
                    $query .= "\n   AND  F.order_detail LIKE '%%유포지%%'";
                }else if($chk_paper == 2){
                    $query .= "\n   AND  F.order_detail LIKE '%%투명데드롱%%' ";
                }else if($chk_paper == 3){
                    $query .= "\n   AND  F.order_detail LIKE '%%은무데드롱%%' ";
                }else if($chk_paper == 4){
                    $query .= "\n   AND  F.order_detail LIKE '%%모조지%%' ";
                }else if($chk_paper == 5){
                    $query .= "\n   AND  F.order_detail LIKE '%%크라프트%%' ";
                }else if($chk_paper == 6){
                    $query .= "\n   AND  F.order_detail LIKE '%%아트지%%' ";
                }
            } else if ($sortcode == "004003009") //자유형도무송 
            {
                $query .= "\n    AND  A.cate_sortcode  = '004003009' ";

                if($chk_paper == 1){
                    $query .= "\n   AND  F.order_detail LIKE '%%유포지%%'";
                }else if($chk_paper == 2){
                    $query .= "\n   AND  F.order_detail LIKE '%%투명데드롱%%' ";
                }else if($chk_paper == 3){
                    $query .= "\n   AND  F.order_detail LIKE '%%은무데드롱%%' ";
                }else if($chk_paper == 4){
                    $query .= "\n   AND  F.order_detail LIKE '%%모조지%%' ";
                }else if($chk_paper == 5){
                    $query .= "\n   AND  F.order_detail LIKE '%%크라프트%%' ";
                }else if($chk_paper == 6){
                    $query .= "\n   AND  F.order_detail LIKE '%%아트지%%' ";
                }
            } else if ($sortcode == "003002001") //명함 유포지 
            {
                $query .= "\n    AND  A.cate_sortcode  = '003002001' ";
                // $query .= "\n   AND F.cate_paper_mpcode != 243 ";

                if($chk_paper == 1){
                    $query .= "\n   AND  F.order_detail LIKE '%%유포지%%' and F.tot_tmpt = '4' " ;
                }else if($chk_paper == 2){
                    $query .= "\n   AND  F.order_detail LIKE '%%유포지%%' and F.tot_tmpt = '8'" ;
                }
            }else if(startsWith($sortcode, "003")) // 명함
            {
                if(startsWith($sortcode, "003008")) {
                    $query .= "\n    AND  (A.cate_sortcode LIKE '003008%%') ";
                } else if(startsWith($sortcode, "003007")) {
                    $query .= "\n    AND  (A.cate_sortcode LIKE '003007%%') ";
                } else if(startsWith($sortcode, "003010")) {
                    $query .= "\n    AND  (A.cate_sortcode LIKE '003010%%') ";
                } else if(startsWith($sortcode, "003011")) {
                    $query .= "\n    AND  (A.cate_sortcode LIKE '003011%%') ";
                } else if(startsWith($sortcode, "003012")) {
                    $query .= "\n    AND  (A.cate_sortcode LIKE '003012%%') ";
                } else if(startsWith($sortcode, "003002002")) {
                    $query .= "\n    AND  (A.cate_sortcode LIKE '003002002') ";
                } else if(startsWith($sortcode, "003002003")) {
                    $query .= "\n    AND  (A.cate_sortcode LIKE '003002003') ";
                } else if(startsWith($sortcode, "003002003")) {
                    $query .= "\n    AND  (A.cate_sortcode LIKE '003002003') ";
                } else if(startsWith($sortcode, "003001")) {
                    $query .= "\n    AND  (A.cate_sortcode LIKE '003001%%') ";
                } else if(startsWith($sortcode, "003002")) {
                    $query .= "\n    AND  (A.cate_sortcode LIKE '003002%%') ";
                } else {
                    $query .= "\n    AND  (A.cate_sortcode LIKE '003%%' ";
                    $query .= "\n    OR A.cate_sortcode LIKE '011001001' ";
                    $query .= "\n    OR A.cate_sortcode LIKE '011003001' ";
                    $query .= "\n    OR A.cate_sortcode LIKE '011003001' ";
                    $query .= "\n    OR A.cate_sortcode LIKE '008001001') ";
                }
            } else if(startsWith($sortcode, "003002001") && $param["order_state"] == "2220") // VIP
            {
                $query .= "\n    AND  (A.cate_sortcode LIKE '003%%' OR A.cate_sortcode = '008001001')";
            } else if(startsWith($sortcode, "008002")) // 자석
            {
                $query .= "\n    AND  (A.cate_sortcode LIKE '008002%%')";
            } else if(startsWith($sortcode, "002001") || startsWith($sortcode, "002002")) // 배너
            {
                $query .= "\n    AND  (A.cate_sortcode LIKE '002001%%' OR A.cate_sortcode LIKE '002002%%')";
            } else if(startsWith($sortcode, "007")) //경인쇄,
            {
                $query .= "\n    AND  (A.cate_sortcode LIKE '007%%' OR A.cate_sortcode LIKE '006002%%')";
            } else if(startsWith($sortcode, "004")) //스티커,
            {
                $query .= "\n    AND  (A.cate_sortcode LIKE '004%%' OR A.cate_sortcode LIKE '011004%%')";
            } else {
                $query .= "\n    AND  A.cate_sortcode LIKE '";
                $query .= substr($param["sortcode"], 1, -1) . "%%'";
            }
        }
        if($search_dvs == "title") {
            if ($this->blankParameterCheck($param, "keyword")) {
                $query .= "\n    AND  A.title LIKE ";
                $query .= "'%%" . substr($param["keyword"], 1, -1) . "%%'";
            }
        }

        if($search_dvs == "order_num") {
            if ($this->blankParameterCheck($param, "keyword")) {
                $query .= "\n    AND  A.order_num = ";
                $query .= "'" . substr($param["keyword"], 1, -1) . "'";
            }
        }

     //   $query .= "\n    AND  F.order_detail LIKE ";
     //   $query .= "'%%" . "스타드림" . "%%'";


        if ($this->blankParameterCheck($param, "cate_paper")) {
            $query .= "\n    AND  F.order_detail LIKE ";
            $query .= "'%%" . substr($param["cate_paper"], 1, -1) . "%%'";
        }

        if($search_dvs == "bun_dlvr_order_num") {
            $query .= "\n    AND  A.member_seqno = " . $param["member_seqno"];
            $query .= "\n    AND  K.bun_dlvr_order_num = " . $param["bun_dlvr_order_num"];
        }

        if ($this->blankParameterCheck($param, "member_name")) {
            $query .= "\n    AND  B.member_name LIKE ";
            $query .= "'%%" . substr($param["member_name"], 1, -1) . "%%'";
        }

        if ($this->blankParameterCheck($param, "version")) {
            $version = substr($param["version"], 1, -1);
            if($version == "3") {
                $query .= "\n    AND  A.OPI_Date IS NOT NULL ";
            } else if($version == "4") {
                $query .= "\n    AND  A.OPI_Date IS NULL ";
            }
        }

        if ($this->blankParameterCheck($param, "title")) {
            $query .= "\n    AND  A.title LIKE ";
            $query .= "'%%" . substr($param["title"], 1, -1) . "%%'";
        }

        if ($this->blankParameterCheck($param, "is_quick")) {
            $query .= "\n    AND  R.opt_name = '당일판' ";
        }

        if ($this->blankParameterCheck($param, "is_quick_today")) {
            $query .= "\n    AND  R.depth1 = '오늘출고' ";
        }

        if ($this->blankParameterCheck($param, "is_quick_tomorrow")) {
            $query .= "\n    AND  R.depth1 = '내일출고' ";
        }

        if ($this->blankParameterCheck($param, "is_auto")) {
            if($param["is_auto"] == "'all'") {
                $query .= "\n    AND  O.empl_id LIKE ";
                $query .= "'auto%%'";
            } else if($param["is_auto"] == "'memo'") {
                $query .= "\n    AND  O.empl_id LIKE ";
                $query .= "'auto%%'";
                $query .= "\n    AND  A.cust_memo != '' ";
            }
        }

        if($search_dvs == "order_mng") {
            if ($this->blankParameterCheck($param, "keyword")) {
                $query .= "\n    AND  P.name LIKE ";
                $query .= "'%%" . substr($param["keyword"], 1, -1) . "%%'";
            }
        }

        if($search_dvs == "invo_num") {
            if ($this->blankParameterCheck($param, "keyword")) {
                $query .= "\n    AND  L.invo_num LIKE ";
                $query .= "'%%" . substr($param["keyword"], 1, -1) . "%%'";
            }
        }

        if($search_dvs == "typset_num") {
            if ($this->blankParameterCheck($param, "keyword")) {
                $query .= "\n    AND  G.typset_num = ";
                $query .= "'" . substr($param["keyword"], 1, -1) . "'";
            }
        }

        if ($this->blankParameterCheck($param, "dlvr_way")) {
            $query .= "\n    AND  L.dlvr_way = " . $param["dlvr_way"];
        }

        if ($this->blankParameterCheck($param, "after")) {
            if($param["after"] == "'Y'")
                $query .= "\n    AND  R.after_name IS NOT NULL AND R.basic_yn = 'N' ";
            else if($param["after"] == "'N'")
                $query .= "\n    AND  R.after_name IS NULL AND R.basic_yn = 'N' ";
            else
                $query .= "\n    AND  R.after_name = " . $param["after"];
        }

        if($order_state != "") {
            if ($this->blankParameterCheck($param, "order_state")) {
                if($order_state == "1320") {
                    $query .= "\n    AND  (A.order_state = " . $param["order_state"] . " OR (A.file_upload_dvs = 'R' AND A.order_state != '1180') OR A.order_state = '1385' OR ((A.OPI_Inserted = 'Y' OR A.OPI_Date IS NULL) AND A.order_state = '1380' AND A.receipt_finish_date < DATE_ADD(NOW(), INTERVAL -10 MINUTE)) ";

                    if($search_dvs == "order_mng") {
                        if ($this->blankParameterCheck($param, "keyword")) {
                            $query .= "OR A.order_state = '1380' OR A.order_state = '2120') ";
                        }
                    } else {
                        $query .= ")";
                    }

                    $query .= "\n    AND  A.file_upload_dvs IN ('R','Y') ";
                } else {
                    $query .= "\n    AND  A.order_state = " . $param["order_state"];
                }
            }
        } else {
            $query .= "\n    AND  A.order_state NOT IN ('1120', '1220')";
        }

        //// 추후 제거
        if($param["empl_id"] == "'dpuser1'") {
            //$query .= "\n    AND  B.member_seqno = 6178 ";
        } else {
            $query .= "\n    AND  B.member_seqno != 6178 ";
        }
        //// 추후 제거
        ///
        if($dvs != "COUNT") {
            $query .= "\n  GROUP BY A.order_common_seqno";
            if($param["order_state"] == "'1320'") {
                $query .= " ORDER BY A.file_upload_dvs ASC, R.opt_name DESC ,A.order_common_seqno ASC";
            } else if($param["order_state"] == "'1380'") {
                $query .= " ORDER BY R.opt_name DESC, A.receipt_finish_date ASC";
            } else {
                $query .= " ORDER BY R.opt_name DESC, A.order_common_seqno DESC";
            }
            $query .= "\n  LIMIT %s, %s";
        }else{
            $query .= "\n  GROUP BY A.order_common_seqno) as at";
        }

        /*
        $query  = sprintf($query, $param["sell_site"]
                                , substr($param["limit_block"], 1, -1)
                                , substr($param["list_size"], 1, -1));
        */
        $query  = sprintf($query
                                , substr($param["limit_block"], 1, -1)
                                , substr($param["list_size"], 1, -1));
       // echo $query;
        //$conn->debug = 1;
       $rs = $conn->Execute($query);
        if($dvs == "COUNT") {
            //echo $query;
            return $rs;
        }
        else
            return makeOrderListHtml($conn, $this, $rs, $param);
            
    }

    /**
     * @brief 주문 리스트 조건검색
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectOrderListCondHtml($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $search_dvs = "A." . $param["search_dvs"];
        $order_state  = $param["order_state"];
        $param = $this->parameterArrayEscape($conn, $param);
        $dvs = substr($param["dvs"], 1, -1);

        $query  = "\n SELECT  A.order_common_seqno AS seqno";
        $query .= "\n        ,C.sell_site";
        $query .= "\n        ,A.order_num";
        $query .= "\n        ,A.order_regi_date";
        $query .= "\n        ,A.depo_finish_date";
        $query .= "\n        ,A.receipt_dvs";
        $query .= "\n        ,B.office_nick";
        $query .= "\n        ,B.member_name";
        $query .= "\n        ,A.title";
        $query .= "\n        ,A.amt";
        $query .= "\n        ,A.amt_unit_dvs";
        $query .= "\n        ,A.count";
        $query .= "\n        ,A.order_mng";
        $query .= "\n        ,A.order_detail";
        $query .= "\n        ,D.erp_state_name";
        $query .= "\n        ,D.state_code";
        $query .= "\n        ,A.OPI_Date";
        $query .= "\n        ,A.pay_way";
        $query .= "\n        ,A.dlvr_dvs";
        $query .= "\n        ,A.oper_sys";
        $query .= "\n        ,A.pay_price";
        $query .= "\n        ,A.add_after_price";
        $query .= "\n        ,A.add_opt_price";
        $query .= "\n        ,B.member_seqno";
        $query .= "\n        ,E.file_path";
        $query .= "\n        ,E.save_file_name";
        $query .= "\n        ,E.origin_file_name";
        $query .= "\n        ,E.size";
        $query .= "\n        ,E.upload_time";
        $query .= "\n        ,E.accept_file_path";
        $query .= "\n        ,E.accept_file_name";
        $query .= "\n        ,E.preview_file_path";
        $query .= "\n        ,E.preview_file_name";
        $query .= "\n        ,A.cust_memo";
        $query .= "\n        ,A.expec_weight";
        $query .= "\n        ,B.mail";
        $query .= "\n        ,B.tel_num";
        $query .= "\n        ,B.cell_num";
        $query .= "\n        ,G.typset_num";
        $query .= "\n        ,CONCAT(I.file_path, I.save_file_name) as typset_pdf ";
        $query .= "\n        ,CONCAT(J.file_path, J.save_file_name) as typset_label";
        $query .= "\n        ,B.sell_channel";
        $query .= "\n   FROM  order_common AS A";
        $query .= "\n   LEFT JOIN order_typset  AS G ON G.order_num = A.order_num ";
        $query .= "\n   LEFT JOIN sheet_typset AS H ON G.typset_num = H.typset_num ";
        $query .= "\n   LEFT JOIN sheet_typset_file AS I ON H.sheet_typset_seqno = I.sheet_typset_seqno ";
        $query .= "\n   LEFT JOIN sheet_typset_label_file AS J ON H.sheet_typset_seqno = J.sheet_typset_seqno ";
        $query .= "\n        ,member       AS B";
        $query .= "\n        ,cpn_admin    AS C";
        $query .= "\n        ,state_admin    AS D";
        $query .= "\n        ,order_file    AS E";
        $query .= "\n        ,order_detail    AS F";


        $query .= "\n  WHERE  A.member_seqno = B.member_seqno";
        $query .= "\n    AND  A.cpn_admin_seqno = C.cpn_admin_seqno";
        $query .= "\n    AND  A.order_state = D.state_code";
        $query .= "\n    AND  A.order_common_seqno = E.order_common_seqno";
        $query .= "\n    AND  A.order_common_seqno = F.order_common_seqno";

        $query .= "\n    AND  A.cpn_admin_seqno =  %s";
        $query .= "\n    AND  A.order_regi_date > %s ";
        $query .= "\n    AND  A.order_regi_date < SYSDATE() ";

        if ($this->blankParameterCheck($param, "sortcode")) {
            $query .= "\n    AND  A.cate_sortcode LIKE '";
            $query .= substr($param["sortcode"], 1, -1) . "%%'";
        }

        if($dvs == "title") {
            if ($this->blankParameterCheck($param, "keyword")) {
                $query .= "\n    AND  A.title LIKE ";
                $query .= "'%%" . substr($param["keyword"], 1, -1) . "%%'";
            }
        }

        if($dvs == "order_num") {
            if ($this->blankParameterCheck($param, "keyword")) {
                $query .= "\n    AND  A.order_num = ";
                $query .= "'" . substr($param["keyword"], 1, -1) . "'";
            }
        }

        if($dvs == "member_name") {
            if ($this->blankParameterCheck($param, "keyword")) {
                $query .= "\n    AND  B.member_name LIKE ";
                $query .= "'%%" . substr($param["keyword"], 1, -1) . "%%'";
            }
        }

        if($dvs == "order_mng") {
            if ($this->blankParameterCheck($param, "keyword")) {
                $query .= "\n    AND  A.order_mng LIKE ";
                $query .= "'%%" . substr($param["keyword"], 1, -1) . "%%'";
            }
        }

        if($dvs == "typset_num") {
            if ($this->blankParameterCheck($param, "keyword")) {
                $query .= "\n    AND  G.typset_num LIKE ";
                $query .= "'%%" . substr($param["keyword"], 1, -1) . "%%'";
            }
        }

        if($order_state != "") {
            if ($this->blankParameterCheck($param, "order_state")) {
                $query .= "\n    AND  A.order_state = " . $param["order_state"];
            }
        } else {
            $query .= "\n    AND  A.order_state NOT IN ('1120', '1220')";
        }

        $query .= "\n  GROUP BY A.order_common_seqno";
        $query .= "\n  ORDER BY A.order_common_seqno DESC";
        $query .= "\n  LIMIT %s, %s";

        $query  = sprintf($query, $param["sell_site"]
            , $param["start_date"]
            , substr($param["limit_block"], 1, -1)
            , substr($param["list_size"], 1, -1));
        $rs = $conn->Execute($query);
        if ($rs->EOF == 1) {
            return false;
        }

        return makeOrderListHtml($conn, $this, $rs);
    }

    /**
     * @brief 주문 리스트 조건검색
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectOrderListCond($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }
        $order_state = $param["order_state"];
        unset($param["order_state"]);

        $search_dvs = "A." . $param["search_dvs"];
        $param = $this->parameterArrayEscape($conn, $param);
        $dvs = substr($param["dvs"], 1, -1);

        $param["order_state"] = $order_state;

        if ($dvs == "COUNT") {
            $query  = "\n SELECT  COUNT(A.order_common_seqno) AS cnt";
        } else if ($dvs == "TOTAL") {
            $query  = "\n SELECT  SUM(A.sell_price) AS sell_sum";
            $query .= "\n        ,SUM(A.add_after_price) AS after_sum";
            $query .= "\n        ,SUM(A.add_opt_price) AS opt_sum";
        }
        $query .= "\n   FROM  order_common AS A";
        $query .= "\n   LEFT JOIN order_typset  AS G ON G.order_num = A.order_num ";
        $query .= "\n        ,member       AS B";
        $query .= "\n        ,cpn_admin    AS C";
        $query .= "\n        ,state_admin    AS D";
        $query .= "\n        ,order_file    AS E";
        $query .= "\n        ,order_detail    AS F";


        $query .= "\n  WHERE  A.member_seqno = B.member_seqno";
        $query .= "\n    AND  A.cpn_admin_seqno = C.cpn_admin_seqno";
        $query .= "\n    AND  A.order_state = D.state_code";
        $query .= "\n    AND  A.order_common_seqno = E.order_common_seqno";
        $query .= "\n    AND  A.order_common_seqno = F.order_common_seqno";

        $query .= "\n    AND  A.cpn_admin_seqno =  %s";
        $query .= "\n    AND  A.order_regi_date > %s ";
        $query .= "\n    AND  A.order_regi_date < SYSDATE() ";

        if ($this->blankParameterCheck($param, "sortcode")) {
            $query .= "\n    AND  A.cate_sortcode LIKE '";
            $query .= substr($param["sortcode"], 1, -1) . "%%'";
        }

        if($dvs == "title") {
            if ($this->blankParameterCheck($param, "keyword")) {
                $query .= "\n    AND  A.title LIKE ";
                $query .= "'%%" . substr($param["keyword"], 1, -1) . "%%'";
            }
        }

        if($dvs == "order_num") {
            if ($this->blankParameterCheck($param, "keyword")) {
                $query .= "\n    AND  A.order_num = ";
                $query .= "'" . substr($param["keyword"], 1, -1) . "'";
            }
        }

        if($dvs == "member_name") {
            if ($this->blankParameterCheck($param, "keyword")) {
                $query .= "\n    AND  B.member_name LIKE ";
                $query .= "'%%" . substr($param["keyword"], 1, -1) . "%%'";
            }
        }

        if($dvs == "order_mng") {
            if ($this->blankParameterCheck($param, "keyword")) {
                $query .= "\n    AND  A.order_mng LIKE ";
                $query .= "'%%" . substr($param["keyword"], 1, -1) . "%%'";
            }
        }

        if($dvs == "typset_num") {
            if ($this->blankParameterCheck($param, "keyword")) {
                $query .= "\n    AND  G.typset_num LIKE ";
                $query .= "'%%" . substr($param["keyword"], 1, -1) . "%%'";
            }
        }

        if($order_state != "") {
            if ($this->blankParameterCheck($param, "order_state")) {
                $query .= "\n    AND  A.order_state = " . $param["order_state"];
            }
        } else {
            $query .= "\n    AND  A.order_state NOT IN ('1120', '1220')";
        }

        $query  = sprintf($query, $param["sell_site"]
            , $param["start_date"]
            , substr($param["limit_block"], 1, -1)
            , substr($param["list_size"], 1, -1));
        return $conn->Execute($query);
    }

    /**
     * @brief 주문별 후공정 가격 검색
     *
     * @param $conn  = connection identifier
     * @param $seqno = 주문 공통 일련번호
     *
     * @return 검색결과
     */
    /*************************************************
    function selectOrderAfterPrice($conn, $seqno) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $temp = array();
        $temp["col"]   = "IFNULL(sum(price), 0) AS price";
        $temp["table"] = "order_after_history";
        $temp["where"]["order_common_seqno"] = $seqno;

        $rs = $this->selectData($conn, $temp);

        return $rs->fields["price"];
    }
    */

    /**
     * @brief 주문별 옵션 가격 검색
     *
     * @param $conn  = connection identifier
     * @param $seqno = 주문 공통 일련번호
     *
     * @return 검색결과
     */
    /*************************************************
    function selectOrderOptPrice($conn, $seqno) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $temp = array();
        $temp["col"]   = "IFNULL(sum(price), 0) AS price";
        $temp["table"] = "order_opt_history";
        $temp["where"]["order_common_seqno"] = $seqno;

        $rs = $this->selectData($conn, $temp);

        return $rs->fields["price"];
    }
    */

    /*
     * 주문 정보 조회
     * $conn : DB Connection
     * $param["seqno"] : 주문공통 일련번호
     * return : resultSet
     */
    function selectOrderInfo($conn, $param) {

        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\nSELECT  A.*, B.*,C.*,D.member_name as member_name2,D.cell_num as cell_num2,D.tel_num as tel_num2 ";
        $query .= "\n  FROM  order_common AS A ";
        $query .= "\n       ,cate AS B ";
        $query .= "\n       ,order_dlvr AS C ";
        $query .= "\n       ,member AS D ";
        //$query .= "\n INNER JOIN member AS D ON A.member_seqno = D.member_seqno ";
        $query .= "\n WHERE  A.cate_sortcode = B.sortcode ";
        $query .= "\n   AND  A.order_common_seqno = C.order_common_seqno AND C.tsrs_dvs = '수신' ";
        $query .= "\n   AND  A.member_seqno = D.member_seqno ";
        if ($this->blankParameterCheck($param, "seqno")) {
            $query .= "\n   AND  A.order_common_seqno = " . $param["seqno"];
        }

        $result = $conn->Execute($query);

        return $result;
    }

    function selectOrderInfo2($conn, $param) {

        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\nSELECT  a.*, b.*,c.*,d.member_name,d.cell_num,d.tel_num ";
        $query .= "\n  FROM  order_common AS A ";
        $query .= "\n       ,cate AS B ";
        $query .= "\n       ,order_dlvr AS C ";
        $query .= "\n       ,member AS D ";
        //$query .= "\n INNER JOIN member AS D ON A.member_seqno = D.member_seqno ";
        $query .= "\n WHERE  A.cate_sortcode = B.sortcode ";
        $query .= "\n   AND  A.order_common_seqno = C.order_common_seqno AND C.tsrs_dvs = '수신' ";
        $query .= "\n   AND  A.member_seqno = D.member_seqno ";
        if ($this->blankParameterCheck($param, "seqno")) {
            $query .= "\n   AND  A.order_common_seqno = " . $param["seqno"];
        }

       // $result = $conn->Execute($query);

       // return $result;
       echo $query;
    }

      function selectPaperInfo($conn, $param) {

        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\nSELECT name ";
        $query .= "\n  FROM  cate_paper ";
        $query .= "\n WHERE  cate_sortcode = ". $param["code"] ." group by name";
        $result = $conn->Execute($query);


        return $this->makePaperOptionHtml($result);


        //return $result;
        //echo $query;
    }

    function selectBunGroupInfo($conn, $param) {

        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\nSELECT D.*, E.erp_state_name,C.dlvr_price,C.dlvr_way, C.dlvr_sum_way, F.name FROM order_common  AS A  ";
        $query .= "\nINNER JOIN order_dlvr AS B ON A.order_common_seqno = B.order_common_seqno AND B.tsrs_dvs = '수신' ";
        $query .= "\nINNER JOIN order_dlvr AS C ON B.bun_dlvr_order_num = C.bun_dlvr_order_num AND C.tsrs_dvs = '수신' ";
        $query .= "\nINNER JOIN order_common AS D ON C.order_common_seqno = D.order_common_seqno ";
        $query .= "\nINNER JOIN state_admin AS E ON E.state_code = D.order_state ";
        if ($this->blankParameterCheck($param, "order_common_seqno")) {
            $query .= "\n   AND  A.order_common_seqno = " . $param["order_common_seqno"];
        }
        $query .= "\nINNER JOIN order_dlvr AS F ON B.bun_dlvr_order_num = F.bun_dlvr_order_num AND F.tsrs_dvs = '송신' ";
        $query .= "\nGROUP by order_common_seqno ";

        $result = $conn->Execute($query);

        return $result;
    }

    function selectOrderAfterInfo($conn, $param) {

        if ($this->connectionCheck($conn) === false) {
            return false;
        }
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT group_concat(CONCAT(A.after_name, ' - ' ,A.depth1 , A.detail) order by A.after_name desc SEPARATOR ' / ') as after_detail,  ";
        $query .= "\n group_concat(CONCAT(A.after_name, '-', IFNULL(B.order_common_seqno,''), '-' ,before_cutting_yn, '-' ,A.depth1, A.detail) order by A.after_name desc SEPARATOR '|') as after_name ";
        $query .= "\n       FROM order_after_history AS A ";
        $query .= "\n       LEFT JOIN order_state_history AS B ON A.order_common_seqno = B.order_common_seqno AND A.after_name = B.detail   ";
        $query .= "\n       WHERE A.order_common_seqno= %s  ";

        $query .= "\n       AND (A.basic_yn='Y' OR A.basic_yn='N' OR A.before_cutting_yn = 'Y')";
        if(strpos($param['sortcode'],'001001') !== false) {
            $query .= "\n       AND (A.basic_yn='Y' OR A.basic_yn='N' OR A.before_cutting_yn = 'Y')";
        } else {
            $query .= "\n       AND (A.basic_yn='N' OR A.before_cutting_yn = 'N')";
        }

        $query .= "\n       GROUP BY A.order_common_seqno ";
        $query .= "\n       ORDER BY A.before_cutting_yn ";

        $query  = sprintf($query, $param["order_common_seqno"]);
        return $conn->Execute($query);
    }

    function selectOrderOptInfo($conn, $param) {

        if ($this->connectionCheck($conn) === false) {
            return false;
        }
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT group_concat(CONCAT(A.opt_name, ' - ' ,A.depth1) order by A.opt_name desc SEPARATOR ' / ') as after_detail,   ";
        $query .= "\n group_concat(CONCAT(A.opt_name, '-', IFNULL(B.order_common_seqno,''), '-' ,A.detail) order by A.opt_name desc SEPARATOR '|') as opt_name  ";
        $query .= "\n        FROM order_opt_history AS A ";
        $query .= "\n       LEFT JOIN order_state_history AS B ON  A.order_common_seqno = B.order_common_seqno AND A.opt_name = B.detail   ";
        $query .= "\n       WHERE A.order_common_seqno= %s  ";

        $query .= "\n       AND (A.basic_yn='Y' OR A.basic_yn='N' )";
        if(strpos($param['sortcode'],'001001') !== false) {
            $query .= "\n       AND (A.basic_yn='Y' OR A.basic_yn='N')";
        } else {
            $query .= "\n       AND (A.basic_yn='N')";
        }

        $query .= "\n       GROUP BY A.order_common_seqno ";

        $query  = sprintf($query, $param["order_common_seqno"]);
        return $conn->Execute($query);
    }

    function selectOrderAfterInfo2($conn, $param) {

        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT group_concat(CONCAT(after_name, ' - ' ,detail) SEPARATOR ' / ') as after_detail ";
        $query .= "\n       FROM order_after_history  ";
        $query .= "\n       WHERE order_common_seqno = %s  ";
        $query .= "\n       AND (basic_yn='N' OR after_name = '접지') ";
        $query .= "\n       GROUP BY order_common_seqno ";

        $query  = sprintf($query, $param["order_common_seqno"]);
        $result = $conn->Execute($query);

        return $result->fields["after_detail"];
    }

    function selectOrderAfterInfoIncludeBasic($conn, $param) {

        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT group_concat(DISTINCT CONCAT(A.after_name, ' - ' ,A.detail, ' - ' ,A.basicyn) order by A.after_name desc SEPARATOR ' / ') as after_detail,  ";
        $query .= "\n group_concat(DISTINCT CONCAT(A.after_name, '-', IFNULL(B.order_common_seqno,''), '-' ,before_cutting_yn, '-' ,A.detail) order by A.after_name desc SEPARATOR '|') as after_name ";
        $query .= "\n       FROM order_after_history AS A ";
        $query .= "\n       LEFT JOIN order_state_history AS B ON A.order_common_seqno = B.order_common_seqno AND A.after_name = B.detail   ";
        $query .= "\n       WHERE A.order_common_seqno= %s  ";

        //$query .= "\n       AND (A.basic_yn='Y' OR A.basic_yn='N' OR A.before_cutting_yn = 'Y')";
        //if(strpos($param['sortcode'],'001001') !== false) {
        //    $query .= "\n       AND (A.basic_yn='Y' OR A.basic_yn='N' OR A.before_cutting_yn = 'Y')";
        //} else {
        //    $query .= "\n       AND (A.basic_yn='N' OR A.before_cutting_yn = 'N')";
        //}

        $query .= "\n       GROUP BY A.order_common_seqno ";
        $query .= "\n       ORDER BY A.before_cutting_yn ";

        $query  = sprintf($query, $param["order_common_seqno"]);
        return $conn->Execute($query);
    }

    function selectOrderHistoryInfo($conn, $param) {

        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT E.name,C.front_state_name, state_modi_date, detail FROM order_common AS A ";
        $query .= "\n       INNER JOIN order_state_history AS B ON A.order_common_seqno = B.order_common_seqno  ";
        $query .= "\n       INNER JOIN state_admin AS C ON B.state = C.state_code  ";
        $query .= "\n       LEFT JOIN empl AS D ON B.empl_id = D.empl_id  ";
        $query .= "\n       LEFT JOIN empl_human_info AS E ON E.empl_seqno = D.empl_seqno  ";
        $query .= "\n       WHERE A.order_common_seqno = %s AND C.front_state_name LIKE '%%완료' ";
        //$query .= "\n       GROUP BY B.state ";
        $query .= "\n       ORDER BY B.state_modi_date asc ";

        $query  = sprintf($query, $param["order_common_seqno"]);
        $result = $conn->Execute($query);

        return $result;
    }

    function selectEstiPaperInfo($conn, $param) {

        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT B.paper_info FROM esti AS A ";
        $query .= "\n       INNER JOIN esti_detail AS B ON A.esti_seqno = B.esti_seqno  ";
        $query .= "\n       WHERE A.order_common_seqno = %s ";

        $query  = sprintf($query, $param["order_common_seqno"]);
        $result = $conn->Execute($query);

        return $result;
    }

    function selectExtnlOrder($conn, $param) {

        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $order_num = $this->arr2paramStr($conn, $param["order_num"]);

        $query  = "\n SELECT * ";
        $query .= "\n FROM order_common AS A ";
        $query .= "\n INNER JOIN member AS B ON A.member_seqno = B.member_seqno ";
        $query .= "\n INNER JOIN order_detail AS C ON A.order_common_seqno = C.order_common_seqno";
        $query .= "\n LEFT JOIN order_file AS D ON A.order_common_seqno = D.order_common_seqno";
        $query .= "\n INNER JOIN order_dlvr AS E ON A.order_common_seqno = E.order_common_seqno AND E.tsrs_dvs = '수신'";
        $query .= "\n WHERE A.order_num IN (%s) ";
        $query .= "\n ORDER BY C.cate_paper_mpcode ";

        $query  = sprintf($query, $order_num);
        $result = $conn->Execute($query);

        return $result;
    }

    function selectExtnlOrder2($conn, $param) {

        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $order_num = $this->arr2paramStr($conn, $param["order_num"]);

        $query  = "\n SELECT A.*, C.*, D.*, E.*, B.cell_num,B.cell_num AS m_cell_num,B.tel_num AS m_tel_num, B.member_name AS m_name, B.addr m_addr, B.addr_detail m_addr_detail ";
        $query .= "\n FROM order_common AS A ";
        $query .= "\n INNER JOIN member AS B ON A.member_seqno = B.member_seqno ";
        $query .= "\n INNER JOIN order_detail AS C ON A.order_common_seqno = C.order_common_seqno";
        $query .= "\n LEFT JOIN order_file AS D ON A.order_common_seqno = D.order_common_seqno";
        $query .= "\n INNER JOIN order_dlvr AS E ON A.order_common_seqno = E.order_common_seqno AND E.tsrs_dvs = '수신'";
        $query .= "\n WHERE A.order_num IN (%s) ";
        $query .= "\n ORDER BY C.cate_paper_mpcode ";

        $query  = sprintf($query, $order_num);
        $result = $conn->Execute($query);

        return $result;
    }

    function selectOrderAfterInfo3($conn, $param) {

        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT group_concat(CONCAT(after_name, '-',detail) SEPARATOR ',') as after_detail ";
        $query .= "\n       FROM order_after_history  ";
        $query .= "\n       WHERE order_common_seqno = %s  ";
        $query .= "\n       AND (basic_yn='N' OR after_name = '접지') ";
        $query .= "\n       GROUP BY order_common_seqno ";

        $query  = sprintf($query, $param["order_common_seqno"]);
        $result = $conn->Execute($query);

        return $result->fields["after_detail"];
    }

    function selectStandAfterInfo($conn, $param) {

        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT * ";
        $query .= "\n       FROM order_after_history  ";
        $query .= "\n       WHERE order_common_seqno = %s AND after_name = '거치대' ";

        $query  = sprintf($query, $param["order_common_seqno"]);
        return $conn->Execute($query);
    }


    function selectEmplName($conn, $param) {

        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT B.name FROM empl AS A ";
        $query .= "\n       INNER JOIN empl_human_info AS B ON A.empl_seqno = B.empl_seqno  ";
        $query .= "\n       WHERE A.empl_id = %s ";

        $query  = sprintf($query, $param["empl_id"]);
        $result = $conn->Execute($query);

        return $result;
    }


    function selectSheetTypsetInfo($conn, $param) {

        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT G.typset_num, CONCAT(I.file_path, I.save_file_name) as typset_pdf, CONCAT(J.file_path, J.save_file_name) as typset_label  ";
        $query .= "\n FROM order_common AS A ";
        $query .= "\n   LEFT JOIN order_typset  AS G ON G.order_num = A.order_num ";
        $query .= "\n   LEFT JOIN sheet_typset AS H ON G.typset_num = H.typset_num ";
        $query .= "\n   LEFT JOIN sheet_typset_file AS I ON H.sheet_typset_seqno = I.sheet_typset_seqno ";
        $query .= "\n   LEFT JOIN sheet_typset_label_file AS J ON H.sheet_typset_seqno = J.sheet_typset_seqno ";
        $query .= "\n   WHERE A.order_common_seqno = %s GROUP by G.typset_num ";

        $query  = sprintf($query, $param["order_common_seqno"]);
        $result = $conn->Execute($query);

        return $result;
    }

    function updateWorkMemo($conn, $param) {

        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\nUPDATE order_common ";
        $query .= "\n       SET work_memo = %s";
        $query .= "\n       WHERE  order_common_seqno = %s ";

        $query  = sprintf($query, $param["work_memo"]
            , $param["order_common_seqno"]);
        $result = $conn->Execute($query);

        return $result;
    }

    function updateAcceptMemo($conn, $param) {

        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\nUPDATE order_common ";
        $query .= "\n       SET accept_memo = %s";
        $query .= "\n       WHERE  order_common_seqno = %s ";

        $query  = sprintf($query, $param["accept_memo"]
            , $param["order_common_seqno"]);
        $result = $conn->Execute($query);

        return $result;
    }

    function selectOrderInfoHistory($conn, $param) {

        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\nSELECT * FROM order_info_history ";
        $query .= "\n       WHERE  order_common_seqno = %s ";

        $query  = sprintf($query,  $param["seq"]);
        $result = $conn->Execute($query);

        return $result;
    }

    function selectAcceptSize($conn, $param) {

        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\nSELECT receipt_size_wid, receipt_size_vert FROM order_detail ";
        $query .= "\n       WHERE  order_common_seqno = %s ";

        $query  = sprintf($query,  $param["order_common_seqno"]);
        $result = $conn->Execute($query);

        return $result;
    }

    function updateAcceptSize($conn, $param) {

        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\nUPDATE order_detail ";
        $query .= "\n       SET receipt_size_wid = %s, receipt_size_vert = %s";
        $query .= "\n       WHERE  order_common_seqno = %s ";

        $query  = sprintf($query, $param["wid"], $param["vert"]
            , $param["order_common_seqno"]);
        $result = $conn->Execute($query);

        return $result;
    }

    function selectPrice($conn, $param) {

        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\nSELECT pay_price FROM order_common ";
        $query .= "\n       WHERE  order_common_seqno = %s ";

        $query  = sprintf($query,  $param["order_common_seqno"]);
        $result = $conn->Execute($query);

        return $result;
    }

    function updatePrice($conn, $param) {

        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\nUPDATE order_common AS A ";
        $query .= "\n       INNER JOIN member_pay_history AS B ON A.order_num = B.order_num AND B.dvs = '매출증가' ";
        $query .= "\n       SET A.pay_price = %s, A.sell_price = %s, B.sell_price = %s, B.pay_price = %s ";
        $query .= "\n       WHERE  A.order_common_seqno = %s ";

        $query  = sprintf($query, $param["pay_price"],$param["pay_price"],$param["pay_price"],$param["pay_price"], $param["order_common_seqno"]);
        $result = $conn->Execute($query);

        return $result;
    }

    function insertOrderInfoHistory($conn, $param) {

        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n INSERT INTO order_info_history ";
        $query .= "\n       (order_common_seqno, kind, before_detail, after_detail, empl_id) VALUES (%s, %s, %s, %s, %s)";

        $query  = sprintf($query, $param["order_common_seqno"], $param["kind"], $param["before"], $param["after"], $param["empl_id"]);
        return $conn->Execute($query);
    }

    /*
     * 주문 배송 정보 조회
     * $conn : DB Connection
     * $param["seqno"] : 주문공통 일련번호
     * return : resultSet
     */
    function selectOrderDlvrInfo($conn, $param) {

        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\nSELECT  tsrs_dvs ";
        $query .= "\n       ,name ";
        $query .= "\n       ,tel_num ";
        $query .= "\n       ,cell_num ";
        $query .= "\n       ,zipcode ";
        $query .= "\n       ,addr ";
        $query .= "\n  FROM  order_dlvr ";
        if ($this->blankParameterCheck($param, "seqno")) {
            $query .= "\n WHERE  order_common_seqno = " . $param["seqno"];
        }

        $result = $conn->Execute($query);

        return $result;
    }

    /**
 * @brief 종이 옵션 html 생성
 *
 */
    function makePaperOptionHtml($rs) {

        $html = "";
    
        while ($rs && !$rs->EOF) {          

            $html .= "\n" . option($rs->fields["name"], $rs->fields["name"]);   

            $rs->MoveNext();
        }

        return $html;
    }
}
?>
