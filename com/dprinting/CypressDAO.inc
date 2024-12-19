<?php
include_once(INC_PATH . '/com/nexmotion/job/nimda/common/NimdaCommonDAO.inc');
/**
 * Created by PhpStorm.
 * User: Hyeonsik Cho
 * Date: 2017-06-23
 * Time: 오후 2:41
 */

class CypressDAO extends NimdaCommonDAO
{
    function __construct() {

    }

    public function checkLoginInfo($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query = "SELECT * /* Cypress - 로그인정보 check */ ";
        $query .= "FROM empl ";
        $query .= "WHERE empl_id = %s AND passwd = PASSWORD(%s) ";


        $query  = sprintf($query, $param['id'], $param['passwd']);
        $rs = $conn->Execute($query);

        if ($rs && !$rs->EOF) {
            return $rs->fields['empl_id'];
        } else {
            return false;
        }
    }

    public function isEnrolledPen($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        //$param = $this->parameterArrayEscape($conn, $param);

        $query = "\n    SELECT sheet_typset_seqno ";
        $query .= "\n   FROM sheet_typset ";
        $query .= "\n   WHERE typset_num like '" . $param['PEN'] . "%' ";

        CLS_File::FileWrite(_WPATH, "\n".date("H:i:s") . $query . "\n", "a+");
        $rs = $conn->Execute($query);

        $sheet_typset_seqnos = array();

        $i = 0;
        while ($rs && !$rs->EOF) {
            array_push($sheet_typset_seqnos, $rs->fields['sheet_typset_seqno']);
            $rs->moveNext();
        }

        return $sheet_typset_seqnos;
    }

    public function resetAlreadyEnrolPenInfo($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query = "\n    UPDATE amt_order_detail_sheet ";
        $query .= "\n   SET sheet_typset_seqno = NULL ";
        $query .= "\n   WHERE sheet_typset_seqno = %s ";

        $query = sprintf($query, $param['sheet_typset_seqno']);
        CLS_File::FileWrite(_WPATH, "\n".date("H:i:s") . $query . "\n", "a+");
        $conn->Execute($query);

        $query = "\n    DELETE FROM sheet_typset_file ";
        $query .= "\n    WHERE sheet_typset_seqno = %s ";

        $query = sprintf($query, $param['sheet_typset_seqno']);
        CLS_File::FileWrite(_WPATH, "\n".date("H:i:s") . $query . "\n", "a+");
        $conn->Execute($query);

        $query = "\n    DELETE FROM sheet_typset_preview_file ";
        $query .= "\n    WHERE sheet_typset_seqno = %s ";

        $query = sprintf($query, $param['sheet_typset_seqno']);
        CLS_File::FileWrite(_WPATH, "\n".date("H:i:s") . $query . "\n", "a+");
        $conn->Execute($query);
    }


    public function deleteAlreadyEnrolPenInfo($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        // 판형 삭제
        $query = "\n    DELETE FROM sheet_typset ";
        $query .= "\n   WHERE sheet_typset_seqno = %s ";

        $query = sprintf($query
            , $param['sheet_typset_seqno']);
        CLS_File::FileWrite(_WPATH, "\n".date("H:i:s") . $query . "\n", "a+");
        $conn->Execute($query);
    }

    public function getAfterProcessInfo($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query = "\n    SELECT A.name ";
        $query .= "\n   FROM after AS A ";
        $query .= "\n   INNER JOIN basic_produce_after AS B ON A.after_seqno = B.after_seqno ";
        $query .= "\n   WHERE B.typset_format_seqno = %s ";

        $query = sprintf($query, $param['typset_format_seqno']);

        $rs = $conn->Execute($query);
        $str = "";
        while ($rs && !$rs->EOF) {
            $str .= $rs->fields['name'].",";
            $rs->moveNext();
        }

        $str = substr($str, 0, strlen($str) - 1);

        return $str;
    }

    public function getEmplSeqno($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query = "\n    SELECT empl_seqno ";
        $query .= "\n    FROM empl ";
        $query .= "\n    WHERE empl_id = %s ";

        $query = sprintf($query, $param['PW']);
        $rs = $conn->Execute($query);

        if($rs && !$rs->EOF) {
            return $rs->fields['empl_seqno'];
        } else {
            return "0";
        }
    }

    public function getOptionInfo($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        for($i = 1; $i <= $param['ONS']; $i++) {
            $query = "\n    SELECT * ";
            $query .= "\n   FROM order_opt_history AS A";
            $query .= "\n   INNER JOIN order_common AS B ON A.order_common_seqno = B.order_common_seqno ";
            $query .= "\n   INNER JOIN order_common AS C ON A.opt_name = C.opt_name ";
            $query .= "\n   WHERE A.basic_yn = 'N' ";
            $query .= "\n   AND A.order_num = %s ";

            $ordernum = substr(substr($param["ON" . $i], 1, strlen($param["ON" . $i])), 0, strlen($param["ON" . $i]) - 4);
            $query = sprintf($query, $ordernum);

            $rs = $conn->Execute($query);

            $str = "";
            $i = 0;
            $special = array();
            while ($rs && !$rs->EOF) {
                $str .= $rs->fields['nick'] . "/";
                $special[$i++] = $rs->fields['nick'];

                $rs->moveNext();
            }

            $rs['option'] = $str;
            $rs['special'] = array_unique($special);

            return $str;
        }
    }

    public function getDlvrboardInfo($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        for($i = 1; $i <= $param['ONS']; $i++) {
            $query = "\n    SELECT ";
            $query .= "\n   FROM order_common AS A";
            $query .= "\n   INNER JOIN order_opt_history AS B ON A.order_common_seqno = B.order_common_seqno ";
            $query .= "\n   INNER JOIN opt_info AS C ON B.opt_name = C.opt_name ";
            $query .= "\n   WHERE A.basic_yn = 'N' ";
            $query .= "\n   AND A.order_num = %s ";

            $order_num = substr($param['ON' . $i], 1, strlen($param['ON' . $i]) - 5);
            $query = sprintf($query, $order_num);

            $rs = $conn->Execute($query);

            $str = "";
            $dlvr_produce_dvs = "";
            while ($rs && !$rs->EOF) {
                $str .= $rs->fields['nick'] . "/";
                $dlvr_produce_dvs = $rs->fields['dlvr_produce_dvs'];
                $rs->moveNext();
            }

            $str = substr($str, 0, strlen($str) - 1);
            $nkData = explode("/", $str);

            for ($n = 0; $n < count($nkData); $n++) {
                if ($nkData[$n] == "당일판") {
                    return "당일판";
                }
            }
        }
        return "본판";
    }

    public function selectOrderDetailFileNum($conn, $barcode_num) {
        $query = " SELECT order_detail_file_num ";
        $query .= " FROM order_detail_count_file ";
        $query .= " WHERE barcode_num = '" . $barcode_num . "'";

        $rs = $conn->Execute($query);
        CLS_File::FileWrite(_WPATH, "\n".date("H:i:s") . $query . "\n", "a+");
        while ($rs && !$rs->EOF) {
            return $rs->fields['order_detail_file_num'];
        }

    }

    public function insertSheetTypset($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $rs = $this->getTypesetFormatInfo($conn, $param);

        $ordPrintetprs = array();
        $ordPrintetprs['국전'] = "2공장-국전";
        $ordPrintetprs['2절'] = "1공장-2절";
        $ordPrintetprs['V3000'] = "1공장-국전V3000";
        $ordPrintetprs['우성프린팅'] = "우성프린팅";
        $ordPrintetprs['원프린팅'] = "원프린팅";
        $ordPrintetprs['현우'] = "현우문화사";
        $ordPrintetprs['DTP'] = "출력실 디지털";
        $ordPrintetprs['디지털'] = "출력실 디지털";
        $ordPrintetprs['요비'] = "1공장-요비";
        $ordPrintetprs['미쓰비시'] = "1공장-2절";

        $insert_param = array();

        if($rs && !$rs->EOF) {
            $insert_param['typset_format_seqno'] = $rs->fields['typset_format_seqno'];
            $insert_param['print_title'] = $rs->fields['preset_name'];
            $insert_param['memo'] = $rs->fields['dscr'];
            $insert_param['cate_sortcode'] = $rs->fields['cate_sortcode'];

            $i = 1;
            CLS_File::FileWrite(_WPATH, "\n".date("H:i:s") . $ordPrintetprs . "\n", "a+");
            while($i <= 3) {
                $print_etprs = $rs->fields['exp' . $i++];
                $insert_param['print_etprs'] = $ordPrintetprs[$print_etprs];
                CLS_File::FileWrite(_WPATH, "\n".date("H:i:s") . $print_etprs . "\n", "a+");
                CLS_File::FileWrite(_WPATH, "\n".date("H:i:s") . $ordPrintetprs[$print_etprs] . "\n", "a+");
                if ($insert_param['print_etprs'] != null || $insert_param['print_etprs'] != "") {
                    break;
                }
            }
        } else {
            echo "Res=41&판정보 읽기 실패";
            exit;
        }

        //$insert_param['typset_num'] = substr($param['PENO'],0, strlen($param['PENO']) - 3);
        $insert_param['typset_num'] = $param['PENO'];
        $insert_param['state'] = "2220";

        if($param['PN'] == 1) {
            $insert_param['beforeside_tmpt'] = $param['PC1'];
            $insert_param['aftside_tmpt'] = "0";
        } else if ($param['PN'] == 2) {
            $insert_param['beforeside_tmpt'] = $param['PC1'];
            $insert_param['aftside_tmpt'] = $param['PC2'];
        } else {
            $insert_param['beforeside_tmpt'] = "0";
            $insert_param['aftside_tmpt'] = "0";
        }
        $insert_param['beforeside_spc_tmpt'] = "0";
        $insert_param['aftside_spc_tmpt'] = "0";

        if($param['PL'] == 2 || $param['PL'] == 3 || $param['PL'] == 4)
            $insert_param['honggak_yn'] = "Y";
        else
            $insert_param['honggak_yn'] = "N";

        $insert_param['after_list'] = $this->getAfterProcessInfo($conn, $param);
        $rs = $this->getOptionInfo($conn, $param);
        $insert_param['opt_list'] = $rs['option'];
        $insert_param['specialty_items'] = $rs['special'];

        $insert_param['print_amt'] = $param['PQ'];
        $insert_param['print_amt_unit'] = "장";
        $insert_param['prdt_page'] = $param['PN'];
        $insert_param['prdt_page_dvs'] = "낱장";
        $insert_param['dlvrboard'] = $this->getDlvrboardInfo($conn, $param);
        $insert_param['op_typ'] = "자동발주";
        $insert_param['op_typ_detail'] = "자동생성";
        $insert_param['empl_seqno'] = $this->getEmplSeqno($conn, $param);

        $rs = $this->getPaperInfo($conn, $param);
        if($rs && !$rs->EOF) {
            $insert_param['paper_name'] = $rs->fields['name'];
            $insert_param['paper_dvs'] = $rs->fields['dvs'];
            $insert_param['paper_color'] = $rs->fields['color'];
            $insert_param['paper_basisweight'] = $rs->fields['basisweight'];
            $insert_param['oper_sys'] = $rs->fields['oper_sys'];
        } else {
            echo "Res=41&종이정보 읽기 실패, ";
            exit;
        }

        $insert_param['regi_date'] = "";
        $insert_param['save_path'] = $param['FILES'];
        $insert_param['save_yn'] = "N";
        $insert_param['typset_way'] = "CYPRESS";

        $query = "\n    INSERT INTO sheet_typset (";
        $query .= "\n   typset_num, state, beforeside_tmpt, beforeside_spc_tmpt, aftside_tmpt, aftside_spc_tmpt, "; //1
        $query .= "\n   honggak_yn, after_list, opt_list, print_amt, print_amt_unit, prdt_page, "; //2
        $query .= "\n   prdt_page_dvs, dlvrboard, memo, op_typ, op_typ_detail, empl_seqno, typset_format_seqno, "; //3
        $query .= "\n   paper_name, paper_dvs, paper_color, paper_basisweight, print_title, regi_date , cate_sortcode, "; //4
        $query .= "\n   oper_sys, save_path, save_yn, specialty_items, typset_way, print_etprs"; //5
        $query .= "\n   ) VALUE (";
        $query .= "\n   %s, %s, %s, %s, %s, %s, "; //1
        $query .= "\n   %s, %s, %s, %s, %s, %s, "; //2
        $query .= "\n   %s, %s, %s, %s, %s, %s, %s, "; //3
        $query .= "\n   %s, %s, %s, %s, %s, %s, %s, "; //4
        $query .= "\n   %s, %s, %s, %s, %s, %s "; //5
        $query .= ")";

        $insert_param = $this->parameterArrayEscape($conn, $insert_param);
        $query = sprintf($query,
            $insert_param['typset_num'], $insert_param['state'], $insert_param['beforeside_tmpt'], $insert_param['beforeside_spc_tmpt'], $insert_param['aftside_tmpt'], $insert_param['aftside_spc_tmpt'],
            $insert_param['honggak_yn'], $insert_param['after_list'], $insert_param['opt_list'], $insert_param['print_amt'], $insert_param['print_amt_unit'], $insert_param['prdt_page'],
            $insert_param['prdt_page_dvs'], $insert_param['dlvrboard'], $insert_param['memo'], $insert_param['op_typ'], $insert_param['op_typ_detail'], $insert_param['empl_seqno'], $insert_param['typset_format_seqno'],
            $insert_param['paper_name'], $insert_param['paper_dvs'], $insert_param['paper_color'], $insert_param['paper_basisweight'], $insert_param['print_title'], 'now()', $insert_param['cate_sortcode'],
            $insert_param['oper_sys'], $insert_param['save_path'], $insert_param['save_yn'], $insert_param['specialty_items'], $insert_param['typset_way'], $insert_param['print_etprs']);

        CLS_File::FileWrite(_WPATH . "_판등록", "\n".date("H:i:s")." insert query -> ".$query."\n", "a+");

        if($conn->Execute($query) !== false) {
            return $insert_param;
        } else {
            return false;
        }
    }


    public function getTypesetFormatInfo($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query = "\n    SELECT * ";
        $query .= "\n    , substring_index(substring_index(preset_name, '_', -2), '_', 1) as exp1 ";
        $query .= "\n    , substring_index(substring_index(preset_name, '_', -3), '_', 1) as exp2 ";
        $query .= "\n    , substring_index(substring_index(preset_name, '_', -1), '_', 1) as exp3 ";
        $query .= "\n   FROM typset_format ";
        $query .= "\n   WHERE preset_name = %s ";

        $query = sprintf($query, $param["PRN"]);
        CLS_File::FileWrite(_WPATH, "\n".date("H:i:s") . $query . "\n", "a+");

        return $conn->Execute($query);
    }

    public function getPaperInfo($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query = "\n SELECT B.oper_sys ,C.name, C.dvs, C.color, C.basisweight ";
        $query .= "\n FROM order_detail AS A";
        $query .= "\n INNER JOIN order_common AS B ON A.order_common_seqno = B.order_common_seqno ";
        $query .= "\n INNER JOIN cate_paper AS C ON A.cate_paper_mpcode = C.mpcode ";
        $query .= "\n WHERE order_detail_dvs_num = '%s' ";

        if($param['ON1'] != null) {
            $orderDetailNum = substr($param['ON1'], 0, strlen($param['ON1']) - 2);
            $query = sprintf($query, $orderDetailNum);
            CLS_File::FileWrite(_WPATH, "\n" . date("H:i:s") . " getpaperinfo query -> " . $query . "\n", "a+");
            return $conn->Execute($query);
        }

        return true;
    }

    public function updateTypsetState($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query = "\n    UPDATE sheet_typset ";
        $query .= "\n   SET state = '%s' ";
        $query .= "\n   WHERE sheet_typset_seqno = %s ";

        $query = sprintf($query, $param["to_state"], $param["sheet_typset_seqno"]);

        return $conn->Execute($query);
    }
    // typset_num과 order_detail_file_num정보가 필요하다
    public function updateState($conn, $param) {
        $param['order_detail_num'] = substr($param['order_detail_file_num'], 0, strlen($param['order_detail_file_num']) - 2);
        $param['order_num'] = substr($param['order_detail_file_num'], 1, strlen($param['order_detail_file_num']) - 5);

        //amt_order_detail_sheet 정보 불러오기
        $param['order_detail_count_file_seqno'] = $this->getOrderDetailCountFileSeqno($conn, $param);
        if(!array_key_exists('sheet_typset_seqno', $param)) {
            $param['sheet_typset_seqno'] = $this->getSheetTypsetSeqno($conn, $param);
        }
        $this->updateAmtOrderDetailSheet($conn, $param);
        //amt_order_detail_sheet테이블에 관련 한 주문의 1건이 전부 생산중 상태인지 확인
        if($this->isAllProductingStateInAmtOrderDetailSheet($conn, $param)) {
            $this->updateProductingStateInOrderDetailCountFile($conn, $param);

            // 한 주문의 전체 건이 전부 생산중 상태인지 확인
            if($this->isAllProductingStateInOrderDetailCountFile($conn, $param)) {
                $this->updateToProductingStateInOrderCommonAndOrderDetail($conn, $param);
            }
        }
    }

    public function getTypsetNum($conn, $param) {
        $query = "\n    SELECT ";
    }

    public function isComposeAvail($conn, $param) {
        $logpath = _WPATH . "_ComposeAvail";

        $query_template = "\n    SELECT order_detail_count_file_seqno ";
        $query_template .= "\n   FROM order_detail_count_file ";
        $query_template .= "\n   WHERE order_detail_file_num = '%s' ";

        for ($i = 1; $i <= $param['ONS']; $i++) {
            $query = sprintf($query_template, $param['ON' . $i]);
            CLS_File::FileWrite($logpath, "\n" . date("H:i:s") . $query, "a+");

            $rs = $conn->Execute($query);
            if (!$rs) {
                CLS_File::FileWrite($logpath, "\n" . date("H:i:s") . "상품이 존재하지 않음", "a+");
                return false;
            }
        }

        return true;
    }

    public function updateToProductingStateInOrderCommonAndOrderDetail($conn, $param){
        $param = $this->parameterArrayEscape($conn, $param);

        $query = "\n    UPDATE order_detail ";
        $query .= "\n   SET state = %s ";
        $query .= "\n   WHERE order_detail_dvs_num = %s ";

        $query = sprintf($query, $param['to_state'],
            $param['order_detail_num']);
        CLS_File::FileWrite(_WPATH, "\n" . date("H:i:s") . $query, "a+");

        if(!$conn->Execute($query)) {
            echo "Res=41&updateToProductingStateInOrderCommonAndOrderDetail order_detail 업데이트 쿼리 실패 ";
            exit;
        }

        $query = "\n    UPDATE order_common ";
        $query .= "\n   SET order_state = %s ";
        $query .= "\n   WHERE order_num = %s ";

        $query = sprintf($query, $param['to_state'],
            $param['order_num']);
        CLS_File::FileWrite(_WPATH, "\n" . date("H:i:s") . $query, "a+");

        if(!$conn->Execute($query)) {
            echo "Res=41&updateToProductingStateInOrderCommonAndOrderDetail order_common 업데이트 쿼리 실패 ";
            exit;
        }

    }

    public function updateProductingStateInOrderDetailCountFile($conn, $param) {
        $param = $this->parameterArrayEscape($conn, $param);

        $query = "\n    UPDATE order_detail_count_file ";
        $query .= "\n   SET state = %s ";
        $query .= "\n   WHERE order_detail_file_num = %s ";

        $query = sprintf($query, $param['to_state'],
            $param['order_detail_file_num']);
        CLS_File::FileWrite(_WPATH, "\n" . date("H:i:s") . $query, "a+");

        $param['before_state'] = $this->selectOrderDetailFileNumState($conn,$param);
        $state_change_query = "\n INSERT INTO state_change_admin ";
        $state_change_query .= "\n (order_detail_file_num, before_state, aft_state) ";
        $state_change_query .= "\n VALUES";
        $state_change_query .= "\n (%s, %s, %s) ";
        $state_change_query =
            sprintf($state_change_query, $param['order_detail_file_num'], $param['before_state'],$param['to_state']);
        $conn->Execute($state_change_query);

        CLS_File::FileWrite(_WPATH, "\n" . date("H:i:s") . $state_change_query, "a+");

        if(!$conn->Execute($query)) {
            echo "Res=41&updateProductingStateInOrderDetailCountFile 쿼리 실패 ";
            exit;
        }
    }

    public function selectOrderDetailFileNumState($conn, $param) {
        $query = "\n    SELECT * FROM order_detail_count_file ";
        $query .= "\n   where order_detail_file_num = " . $param['order_detail_file_num'];

        $rs = $conn->Execute($query);
        CLS_File::FileWrite(_WPATH, "\n" . date("H:i:s") . $query, "a+");
        while($rs && !$rs->EOF) {
            return $rs->fields['state'];
        }
    }

    public function getOrderDetailCountFileSeqno($conn, $param) {
        $param = $this->parameterArrayEscape($conn, $param);

        $query = "\n    SELECT order_detail_count_file_seqno ";
        $query .= "\n   FROM order_detail_count_file ";
        $query .= "\n  WHERE order_detail_file_num = %s ";

        $query = sprintf($query, $param['order_detail_file_num']);
        CLS_File::FileWrite(_WPATH, "\n" . date("H:i:s") . $query, "a+");

        $rs = $conn->Execute($query);

        if($rs && !$rs->EOF) {
            return $rs->fields['order_detail_count_file_seqno'];
        } else {
            echo "Res=41&getOrderDetailCountFileSeqno 쿼리 실패 ";
            exit;
        }
    }

    public function getSheetTypsetSeqno($conn, $param) {
        $param = $this->parameterArrayEscape($conn, $param);

        $query = "\n    SELECT sheet_typset_seqno ";
        $query .= "\n   FROM sheet_typset ";
        $query .= "\n  WHERE typset_num = %s ";

        $query = sprintf($query, $param['typset_num']);
        CLS_File::FileWrite(_WPATH, "\n" . date("H:i:s") . $query, "a+");

        $rs = $conn->Execute($query);

        if($rs && !$rs->EOF) {
            return $rs->fields['sheet_typset_seqno'];
        } else {
            return NULL;
        }
    }

    public function updateAmtOrderDetailSheet($conn, $param) {
        /*
            현재 PlateItemAdd의 파라미터로는 매수분활 된 상품을 구분 할 수 없기때문에
            일단 매수가 없을 시 매수정보조건 없이 빼고 업데이트
        */
        if(array_key_exists('PQ',$param)) {
            $idx = $param['idx'];
            $amt = $param['OP'. $idx] * $param['PQ'];
            $query = "\n    UPDATE amt_order_detail_sheet ";
            $query .= "\n   SET state = %s, sheet_typset_seqno = %s ";
            $query .= "\n   WHERE order_detail_count_file_seqno = %s ";
            $query .= "\n   LIMIT 1";
            // $query .= "\n   AND amt = %s "; 200매짜리를 500매짜리 판에서찍을때 문제 발생가능
        } else {
            $query = "\n    UPDATE amt_order_detail_sheet ";
            $query .= "\n   SET state = %s ";
            $query .= "\n   WHERE order_detail_count_file_seqno = %s ";
            $query .= "\n   LIMIT 1";
        }

        if(array_key_exists('PQ',$param)) {
            $param = $this->parameterArrayEscape($conn, $param);
            $query = sprintf($query, $param['to_state'],$param['sheet_typset_seqno'],
                $param['order_detail_count_file_seqno']);
        } else {
            $param = $this->parameterArrayEscape($conn, $param);
            $query = sprintf($query, $param['to_state'],
                $param['order_detail_count_file_seqno']);
        }


        CLS_File::FileWrite(_WPATH, "\n" . date("H:i:s") . $query, "a+");
        if(!$conn->Execute($query)) {
            echo "Res=41&updateAmtOrderDetailSheet 쿼리 실패 ";
            exit;
        }
    }

    public function isAllProductingStateInAmtOrderDetailSheet($conn, $param) {
        $to_state = $param['to_state'];
        $param = $this->parameterArrayEscape($conn, $param);

        $query = "\n    SELECT state ";
        $query .= "\n    FROM amt_order_detail_sheet ";
        $query .= "\n   WHERE order_detail_count_file_seqno = %s ";

        $query = sprintf($query, $param['order_detail_count_file_seqno']);
        CLS_File::FileWrite(_WPATH, "\n" . date("H:i:s") . $query, "a+");

        $rs = $conn->Execute($query);

        if(!$rs) {
            echo "Res=41&isAllProductingStateInAmtOrderDetailSheet 쿼리 실패 ";
            exit;
        }

        while ($rs && !$rs->EOF) {
            CLS_File::FileWrite(_WPATH, "\n" . date("H:i:s") . "state : " . $rs->fields['state'], "a+");
            CLS_File::FileWrite(_WPATH, "\n" . date("H:i:s") . "to_state : " .$param['to_state'], "a+");
            if($rs->fields['state'] < $to_state) {
                return false;
            }
            $rs->MoveNext();
        }

        return true;
    }

    public function isAllProductingStateInOrderDetailCountFile($conn, $param) {
        $query = "\n    SELECT state ";
        $query .= "\n    FROM order_detail_count_file ";
        $query .= "\n   WHERE order_detail_file_num like '". $param['order_detail_num'] ."%' ";

        CLS_File::FileWrite(_WPATH, "\n" . date("H:i:s") . $query, "a+");

        $rs = $conn->Execute($query);

        if(!$rs) {
            echo "Res=41&isAllProductingStateInOrderDetailCountFile 쿼리 실패 ";
            exit;
        }

        while ($rs && !$rs->EOF) {
            if($rs->fields['state'] < $param['to_state']) {
                return false;
            }
            $rs->MoveNext();
        }

        return true;
    }
}