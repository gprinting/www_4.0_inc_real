<?php
/**
 * Created by PhpStorm.
 * User: Hyeonsik Cho
 * Date: 2018-01-24
 * Time: 오전 11:53
 */

include_once(INC_PATH . '/com/nexmotion/job/front/common/FrontCommonDAO.inc');

class MoamoaDAO extends FrontCommonDAO
{
    function __construct()
    {
    }

    public function selectProductInfo($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }
        $query =  " SELECT \n";
        $query .= " A.order_common_seqno AS order_common_seqno, \n";
        $query .= " 'GP' as BranchShop \n";
        $query .= " ,B.member_name as UCom \n";
        $query .= " ,A.title as Name \n";
        $query .= " ,A.order_num as OrderNum \n";
        $query .= " ,A.depo_finish_date as PurchaseDate \n";
        $query .= " ,A.dlvr_produce_dvs as RDiv \n";
        $query .= " ,C.high_sortcode AS ItemCode \n";
        $query .= " ,SUBSTRING_INDEX(tot_name, '|', 1) AS ItemName \n";
        $query .= " ,C.sortcode AS ProductCode \n";
        $query .= " ,C.cate_name AS ProductName \n";
        $query .= " ,B.dlvr_add_info AS BaseDelivery \n";
        $query .= " ,'A 9' AS Method \n";
        $query .= " ,A.page_cnt AS QuantityCode \n";
        $query .= " ,CONCAT(CAST(A.amt AS signed integer), A.amt_unit_dvs) AS QuantityName \n";
        $query .= " ,A.page_cnt AS QuantityValue \n";
        $query .= " ,A.count AS CaseCode \n";
        $query .= " ,CONCAT(A.count,'건') AS CaseName \n";
        $query .= " ,A.count AS CaseValue \n";
        $query .= " ,E.mpcode AS PaperCode \n";
        $query .= " ,CONCAT(E.name,' ',E.color,' ',E.basisweight) AS PaperName \n";
        $query .= " ,E.dvs AS PaperDvs \n";
        $query .= " ,D.side_dvs AS Side \n";
        $query .= " ,D.stan_name AS SizeCode \n";
        $query .= " ,D.stan_name AS SizeName \n";
        $query .= " ,D.cut_size_wid AS PWidth \n";
        $query .= " ,D.cut_size_vert AS PHeight \n";
        $query .= " ,D.receipt_size_wid AS Width \n";
        $query .= " ,D.receipt_size_vert AS Height \n";
        $query .= " ,A.cust_memo AS Memo \n";
        $query .= " ,A.work_memo AS WMemo \n";
        $query .= " ,F.origin_file_name AS FileName\n";
        $query .= " ,CONCAT(F.file_path, F.save_file_name) AS FilePath \n";
        $query .= " ,D.tot_tmpt AS Tmpt, F.size, F.origin_file_name , A.member_seqno \n";
        $query .= " ,A.order_detail AS Detail\n";
        $query .= " ,A.receipt_finish_date AS ReceiptDate\n";
        $query .= " ,G.dlvr_way \n";
        $query .= " ,G.dlvr_sum_way \n";
        $query .= " ,G.invo_cpn \n";
        $query .= " ,A.amt \n";
        $query .= " ,A.amt_unit_dvs \n";
        $query .= " ,A.OPI_Date \n";
        $query .= " ,A.OPI_Seq \n";
        $query .= " ,A.order_state \n";
        $query .= " ,D.stan_name \n";
        $query .= " FROM order_common AS A \n";
        $query .= " INNER JOIN member AS B ON A.member_seqno = B.member_seqno \n";
        $query .= " INNER JOIN cate AS C ON A.cate_sortcode = C.sortcode \n";
        $query .= " INNER JOIN order_detail AS D ON A.order_common_seqno = D.order_common_seqno \n";
        $query .= " LEFT JOIN cate_paper AS E ON E.mpcode = D.cate_paper_mpcode \n";
        $query .= " INNER JOIN order_file AS F ON A.order_common_seqno = F.order_common_seqno \n";
        $query .= " LEFT JOIN order_dlvr AS G ON A.order_common_seqno = G.order_common_seqno AND G.tsrs_dvs = '수신' \n";
        $query .= " WHERE A.order_num = '%s' AND A.order_state != '1180' \n";
        $query = sprintf($query, $param['ordernum']);
        $rs = $conn->Execute($query);
        return $rs->fields;
    }

    public function selectProductInfoTest($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }
        $query =  " SELECT \n";
        $query .= " A.order_common_seqno AS order_common_seqno, \n";
        $query .= " 'GP' as BranchShop \n";
        $query .= " ,B.member_name as UCom \n";
        $query .= " ,A.title as Name \n";
        $query .= " ,A.order_num as OrderNum \n";
        $query .= " ,A.depo_finish_date as PurchaseDate \n";
        $query .= " ,A.dlvr_produce_dvs as RDiv \n";
        $query .= " ,C.high_sortcode AS ItemCode \n";
        $query .= " ,SUBSTRING_INDEX(tot_name, '|', 1) AS ItemName \n";
        $query .= " ,C.sortcode AS ProductCode \n";
        $query .= " ,C.cate_name AS ProductName \n";
        $query .= " ,'A 9' AS BaseDelivery \n";
        $query .= " ,'A 9' AS Method \n";
        $query .= " ,A.page_cnt AS QuantityCode \n";
        $query .= " ,CONCAT(CAST(A.amt AS signed integer), A.amt_unit_dvs) AS QuantityName \n";
        $query .= " ,A.page_cnt AS QuantityValue \n";
        $query .= " ,A.count AS CaseCode \n";
        $query .= " ,CONCAT(A.count,'건') AS CaseName \n";
        $query .= " ,A.count AS CaseValue \n";
        $query .= " ,E.mpcode AS PaperCode \n";
        $query .= " ,CONCAT(E.name,' ',E.color,' ',E.basisweight) AS PaperName \n";
        $query .= " ,E.dvs AS PaperDvs \n";
        $query .= " ,D.side_dvs AS Side \n";
        $query .= " ,D.stan_name AS SizeCode \n";
        $query .= " ,D.stan_name AS SizeName \n";
        $query .= " ,D.cut_size_wid AS PWidth \n";
        $query .= " ,D.cut_size_vert AS PHeight \n";
        $query .= " ,D.receipt_size_wid AS Width \n";
        $query .= " ,D.receipt_size_vert AS Height \n";
        $query .= " ,A.cust_memo AS Memo \n";
        $query .= " ,A.work_memo AS WMemo \n";
        $query .= " ,F.origin_file_name AS FileName\n";
        $query .= " ,CONCAT(F.file_path, F.save_file_name) AS FilePath \n";
        $query .= " ,D.tot_tmpt AS Tmpt, F.size, F.origin_file_name , A.member_seqno \n";
        $query .= " ,A.order_detail AS Detail\n";
        $query .= " ,A.receipt_finish_date AS ReceiptDate\n";
        $query .= " ,G.dlvr_way \n";
        $query .= " ,G.invo_cpn \n";
        $query .= " FROM order_common AS A \n";
        $query .= " INNER JOIN member AS B ON A.member_seqno = B.member_seqno \n";
        $query .= " INNER JOIN cate AS C ON A.cate_sortcode = C.sortcode \n";
        $query .= " INNER JOIN order_detail AS D ON A.order_common_seqno = D.order_common_seqno \n";
        $query .= " LEFT JOIN cate_paper AS E ON E.mpcode = D.cate_paper_mpcode \n";
        $query .= " INNER JOIN order_file AS F ON A.order_common_seqno = F.order_common_seqno \n";
        $query .= " INNER JOIN order_dlvr AS G ON A.order_common_seqno = G.order_common_seqno AND G.tsrs_dvs = '수신' \n";
        $query .= " WHERE A.order_num = '%s' \n";
        $query = sprintf($query, $param['ordernum']);
        $rs = $conn->Execute($query);
        return $rs->fields;
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

    function updateProductHold($conn, $param) {

        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n UPDATE order_common SET order_mng = '' WHERE order_num = %s ";

        $query  = sprintf($query, $param["ordernum"]);
        $result = $conn->Execute($query);

        return $result;
    }

    function updateTitle($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n UPDATE order_common SET title = %s WHERE order_num = %s ";

        $query  = sprintf($query, $param["title"], $param["ordernum"]);
        $result = $conn->Execute($query);

        return $result;
    }

    function selectAworksInfo($conn, $param) {

        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT * FROM order_after_history ";
        $query .= "\n       WHERE A.order_common_seqno = %s AND basic_yn = 'N' ";

        $query  = sprintf($query, $param["order_common_seqno"]);
        $result = $conn->Execute($query);

        return $result;
    }

    public function selectAutoProductInfo($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }
        $query =  " SELECT \n";
        $query .= " A.order_common_seqno AS order_common_seqno \n";
        $query .= " ,B.member_seqno AS member_seqno \n";
        $query .= " ,C.sortcode AS ProductCode \n";
        $query .= " ,C.cate_name AS ProductName \n";
        $query .= " ,A.count AS CaseCode \n";
        $query .= " ,D.cut_size_wid AS Width \n";
        $query .= " ,D.cut_size_vert AS Height \n";
        $query .= " ,D.receipt_size_wid AS PWidth \n";
        $query .= " ,D.receipt_size_vert AS PHeight \n";
        $query .= " ,A.oper_sys AS oper_sys \n";
        $query .= " ,F.origin_file_name AS FileName\n";
        $query .= " ,CONCAT(F.file_path, F.save_file_name) AS FilePath \n";
        $query .= " ,D.tot_tmpt AS Tmpt \n";
        $query .= " ,E.mpcode AS PaperCode \n";
        $query .= " ,A.page_cnt AS QuantityValue \n";
        $query .= " ,D.stan_name AS SizeCode \n";
        $query .= " ,B.member_name as UCom \n";
        $query .= " ,'A 9' AS Method \n";
        $query .= " ,A.order_num \n";
        $query .= " FROM order_common AS A \n";
        $query .= " INNER JOIN member AS B ON A.member_seqno = B.member_seqno \n";
        $query .= " INNER JOIN cate AS C ON A.cate_sortcode = C.sortcode \n";
        $query .= " INNER JOIN order_detail AS D ON A.order_common_seqno = D.order_common_seqno \n";
        $query .= " INNER JOIN cate_paper AS E ON E.mpcode = D.cate_paper_mpcode \n";
        $query .= " INNER JOIN order_file AS F ON A.order_common_seqno = F.order_common_seqno \n";
        //GPT220518BL00290
        //$query .= " WHERE order_num = 'GPT220523BL00164' \n";
        $query .= " WHERE A.order_mng = '%s' AND A.order_state = '1330' AND receipt_dvs = 'Auto' \n";
        $query = sprintf($query, $param['machine_id']);

        $rs = $conn->Execute($query);
        return $rs->fields;
    }

    public function selectAutoProductInfo_TEST($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }
        $query =  " SELECT \n";
        $query .= " A.order_common_seqno AS order_common_seqno \n";
        $query .= " ,B.member_seqno AS member_seqno \n";
        $query .= " ,C.sortcode AS ProductCode \n";
        $query .= " ,C.cate_name AS ProductName \n";
        $query .= " ,A.count AS CaseCode \n";
        $query .= " ,D.cut_size_wid AS Width \n";
        $query .= " ,D.cut_size_vert AS Height \n";
        $query .= " ,D.receipt_size_wid AS PWidth \n";
        $query .= " ,D.receipt_size_vert AS PHeight \n";
        $query .= " ,A.oper_sys AS oper_sys \n";
        $query .= " ,F.origin_file_name AS FileName\n";
        $query .= " ,CONCAT(F.file_path, F.save_file_name) AS FilePath \n";
        $query .= " ,D.tot_tmpt AS Tmpt \n";
        $query .= " ,E.mpcode AS PaperCode \n";
        $query .= " ,A.page_cnt AS QuantityValue \n";
        $query .= " ,D.stan_name AS SizeCode \n";
        $query .= " ,B.member_name as UCom \n";
        $query .= " ,'A 9' AS Method \n";
        $query .= " ,A.order_num \n";
        $query .= " FROM order_common AS A \n";
        $query .= " INNER JOIN member AS B ON A.member_seqno = B.member_seqno \n";
        $query .= " INNER JOIN cate AS C ON A.cate_sortcode = C.sortcode \n";
        $query .= " INNER JOIN order_detail AS D ON A.order_common_seqno = D.order_common_seqno \n";
        $query .= " INNER JOIN cate_paper AS E ON E.mpcode = D.cate_paper_mpcode \n";
        $query .= " INNER JOIN order_file AS F ON A.order_common_seqno = F.order_common_seqno \n";
        $query .= " WHERE A.order_num = 'GPT220726NC01499' \n";
        //$query = sprintf($query, $param['machine_id']);

        $rs = $conn->Execute($query);
        return $rs->fields;
    }

    function UpdateAutoResultSuccess($conn, $param) {

        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n UPDATE order_common ";
        $query .= "\n       SET  order_state = '1380'  ";
        $query .= "\n       WHERE order_common_seqno = %s AND order_state != '1180' ";

        $query  = sprintf($query, $param["order_common_seqno"]);
        $conn->Execute($query);

        $query  = "\n UPDATE order_common ";
        $query .= "\n       SET work_memo = cust_memo  ";
        $query .= "\n       WHERE order_common_seqno = %s AND cate_sortcode = '005001001' ";
        $query  = sprintf($query, $param["order_common_seqno"]);
        return $conn->Execute($query);
    }

    function UpdateAutoResultFail($conn, $param) {

        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n UPDATE order_common AS B ";
        $query .= "\n       SET  order_state = '1320' ";
        $query .= "\n       WHERE order_common_seqno = %s AND order_state != '1180' ";
        $query  = sprintf($query, $param["order_common_seqno"]);
        $conn->Execute($query);

        $query  = "\n UPDATE order_common AS B ";
        $query .= "\n       SET  order_mng = '', receipt_dvs = 'Manual' ";
        $query .= "\n       WHERE order_common_seqno = %s AND order_mng LIKE 'auto%%'  ";
        $query  = sprintf($query, $param["order_common_seqno"]);
        $conn->Execute($query);

        $query  = "\n INSERT INTO auto_error_history (order_common_seqno, error_code) ";
        $query .= "\n       VALUES (%s, %s) ";
        $query  = sprintf($query, $param["order_common_seqno"], $param["error_list"]);
        $conn->Execute($query);
    }

    function selectCoatingInfo($conn, $param) {

        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT after_name ";
        $query .= "\n       FROM order_after_history  ";
        $query .= "\n       WHERE order_common_seqno = %s  ";
        $query .= "\n       AND after_name = '코팅' ";

        $query  = sprintf($query, $param["order_common_seqno"]);
        $result = $conn->Execute($query);

        return $result->fields["after_name"];
    }

    function selectOrderAfterInfo($conn, $param) {

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

    function selectOrderOptInfo($conn, $param) {

        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT group_concat(CONCAT(opt_name) SEPARATOR ' / ') as opt_detail ";
        $query .= "\n       FROM order_opt_history  ";
        $query .= "\n       WHERE order_common_seqno = %s  ";
        //$query .= "\n       AND (basic_yn='N' OR after_name = '접지') ";
        //$query .= "\n       GROUP BY order_common_seqno ";

        $query  = sprintf($query, $param["order_common_seqno"]);
        $result = $conn->Execute($query);

        return $result->fields["opt_detail"];
    }

    function selectOrderAfterInfoByOrderNum($conn, $param) {

        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT group_concat(CONCAT(B.after_name, ' - ' ,B.detail) SEPARATOR ' / ') as after_detail  ";
        $query .= "\n       FROM order_common AS A  ";
        $query .= "\n       INNER JOIN order_after_history AS B ON A.order_common_seqno = B.order_common_seqno  ";
        $query .= "\n       WHERE A.order_num = %s  ";
        $query .= "\n       AND (B.basic_yn='N' OR B.after_name = '봉투가공' OR B.after_name = '접지') ";
        $query .= "\n       GROUP BY A.order_common_seqno ";

        $query  = sprintf($query, $param["ordernum"]);
        $result = $conn->Execute($query);

        return $result->fields["after_detail"];
    }

    function selectOneFile($conn, $param) {

        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT GROUP_CONCAT(B.order_num SEPARATOR '|') AS order_num ";
        $query .= "\n       FROM order_file AS A   ";
        $query .= "\n       INNER JOIN order_common AS B ON A.order_common_seqno = B.order_common_seqno  ";
        $query .= "\n       WHERE A.size = %s  ";
        $query .= "\n       AND A.member_seqno = %s  ";
        $query .= "\n       AND A.origin_file_name = %s  ";
        $query .= "\n       AND B.order_num != %s  ";
        $query .= "\n       AND B.order_state = '1320'  ";

        $query  = sprintf($query, $param["size"]
            , $param["member_seqno"]
            , $param["origin_file_name"]
            , $param["ordernum"]);
        $result = $conn->Execute($query);

        return $result->fields["order_num"];
    }

    function selectPassword($conn, $pw) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $pw = $this->parameterEscape($conn, $pw);

        $query = "select PASSWORD(%s) AS pw;";
        $query = sprintf($query, $pw);

        $rs = $conn->Execute($query);

        return $rs->fields["pw"];
    }

    function selectEmpl($conn, $id) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $id = $this->parameterEscape($conn, $id);

        $query  = "\nSELECT B.enter_date ";  /* 입사일 */
        $query .= "\n      ,A.passwd ";      /* 비밀번호 */
        $query .= "\n      ,A.empl_seqno ";  /* 일련번호 */
        $query .= "\n      ,A.depar_code ";  /* 직원 부서코드 */
        $query .= "\n      ,B.name ";        /* 직원 이름 */
        $query .= "\n  FROM empl AS A ";
        $query .= "\n      ,empl_human_info AS B ";
        $query .= "\n WHERE A.empl_seqno = B.empl_seqno ";
        $query .= "\n   AND A.empl_id = %s ";

        $query  = sprintf($query, $id);

        return $conn->Execute($query);
    }

    public function insertStateHistory($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param['date'] = "NOW()";
        if($param['state'] == '2680') {
            $param['date'] = "DATE_ADD(NOW(), INTERVAL 5 SECOND)";
        }

        $query = " INSERT INTO order_state_history (order_common_seqno, state, state_modi_date, empl_id, detail) ";
        $query .= " SELECT order_common_seqno, '%s', %s, '%s', '%s' FROM order_common ";
        $query .= " WHERE order_num = '%s' ";

        $query = sprintf($query, $param['state'], $param['date'], $param['empl_id'],$param['detail'],$param['ordernum']);

        $rs = $conn->Execute($query);
        return $rs->fields;
    }

    public function insertMigration($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query = " INSERT INTO migration (OPI_Date, OPI_Seq, state3) ";
        $query .= " VALUES ('%s', '%s', %s) ";

        $query = sprintf($query, $param['OPI_Date'], $param['OPI_Seq'],$param['state3']);

        $rs = $conn->Execute($query);
        return $rs->fields;
    }

    public function updateDlvrInfo($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query = " UPDATE order_dlvr AS A INNER JOIN order_common AS B ";
        $query .= " ON A.order_common_seqno = B.order_common_seqno ";
        $query .= " SET A.invo_num = '%s' ";
        $query .= " WHERE B.OPI_Date = '%s' AND B.OPI_Seq = '%s'";

        $query = sprintf($query, $param['serialnum'], $param['OPI_Date'],$param['OPI_Seq']);

        $rs = $conn->Execute($query);
        return $rs->fields;
    }

    public function select_30OrderNum($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query = " SELECT A.OPI_Date, A.OPI_Seq, A.OPI_Inserted, A.count, A.cate_sortcode, A.member_seqno, A.order_mng, A.order_state, B.cate_paper_mpcode FROM order_common AS A ";
        $query .= " INNER JOIN order_detail AS B ON A.order_common_seqno = B.order_common_seqno ";
        $query .= " WHERE A.order_num = '%s' ";

        $query = sprintf($query, $param['ordernum']);

        $rs = $conn->Execute($query);
        return $rs->fields;
    }

    public function delete_30OrderNum($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query = " UPDATE migration SET is_treated = 'Y' ";
        $query .= " WHERE OPI_Date = '%s' AND OPI_Seq = '%s' ";

        $query = sprintf($query, $param['OPI_Date'],  $param['OPI_Seq']);

        $rs = $conn->Execute($query);
        return $rs->fields;
    }

    public function update_30Insert($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query = " UPDATE order_common SET OPI_Inserted = 'Y' ";
        $query .= " WHERE order_num = '%s' ";

        $query = sprintf($query, $param['ordernum']);

        $rs = $conn->Execute($query);
        return $rs->fields;
    }

    public function updateProductState($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }
        $query =  " UPDATE order_common AS A  \n";
        $query .= " INNER JOIN order_detail B ON A.order_common_seqno = B.order_common_seqno \n";
        $query .= " INNER JOIN order_file C ON A.order_common_seqno = C.order_common_seqno \n";
        $query .= " SET A.order_state = '%s', B.state = '%s' , C.accept_file_path = '%s' , C.accept_file_name = '%s' \n";
        $query .= " ,C.preview_file_path = '%s' , C.preview_file_name = '%s', A.receipt_finish_date = NOW() \n";
        $query .= " WHERE A.order_num = '%s' AND A.order_state <= '2120' AND A.order_state != '1180' \n";
        $query = sprintf($query, $param['state'], $param['state'], $param['accept_file_path'],
            $param['accept_file_name'], $param['preview_file_path'],
            $param['preview_file_name'], $param['ordernum']);

        $rs = $conn->Execute($query);
        return $rs->fields;
    }

    public function updateProductStatecode($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query =  " UPDATE order_common AS A  \n";
        $query .= " INNER JOIN order_detail B ON A.order_common_seqno = B.order_common_seqno \n";
        $query .= " SET A.order_state = '%s', B.state = '%s' \n";
        $query .= " WHERE A.order_num = '%s' AND A.order_state != '1180' \n";
        $query = sprintf($query, $param['state'], $param['state'], $param['ordernum']);

        $rs = $conn->Execute($query);
        return $rs->fields;
    }

    public function updateProductStatecodeByReceipt($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query =  " UPDATE order_common AS A  \n";
        $query .= " INNER JOIN order_detail B ON A.order_common_seqno = B.order_common_seqno \n";
        $query .= " SET A.order_state = '%s', B.state = '%s', A.receipt_finish_date = NOW() \n";
        $query .= " WHERE A.order_num = '%s' AND A.order_state != '1180' AND A.order_state < '2220' \n";
        $query = sprintf($query, $param['state'], $param['state'], $param['ordernum']);

        $rs = $conn->Execute($query);
        return $rs->fields;
    }

    public function updateProductStatecodeByReceiptTEST($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query =  " UPDATE order_common AS A  \n";
        $query .= " INNER JOIN order_detail B ON A.order_common_seqno = B.order_common_seqno \n";
        $query .= " SET A.order_state = '%s', B.state = '%s', A.receipt_finish_date = NOW() \n";
        $query .= " WHERE A.order_num = '%s' AND A.order_state != '1180' AND A.order_state > '' \n";
        $query = sprintf($query, $param['state'], $param['state'], $param['ordernum']);

        $rs = $conn->Execute($query);
        return $rs->fields;
    }

    public function updateReupload($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query =  " UPDATE order_common AS A  \n";
        $query .= " SET A.file_upload_dvs = 'Y' \n";
        $query .= " WHERE A.order_num = '%s' \n";
        $query = sprintf($query, $param['ordernum']);

        $rs = $conn->Execute($query);
        return $rs->fields;
    }

    public function selectOPIInfo($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query =  " SELECT OPI_Date, OPI_Seq FROM order_common  \n";
        $query .= " WHERE order_num = '%s' \n";
        $query = sprintf($query, $param['ordernum']);

        $rs = $conn->Execute($query);
        return $rs;
    }

    public function updateReceiptMemo($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query =  " UPDATE order_common AS A  \n";
        $query .= " SET accept_memo = '%s' \n";
        $query .= " WHERE order_num = '%s' \n";
        $query = sprintf($query, $param['receipt_memo'], $param['ordernum']);

        $rs = $conn->Execute($query);
        return $rs->fields;
    }

    public function selectOrderNumBy2JumunNo($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query =  " SELECT order_num FROM order_common AS A  \n";
        $query .= " INNER JOIN migration_by_case B ON A.OPI_Date = B.OPI_Date AND A.OPI_Seq = B.OPI_Seq \n";
        $query .= " WHERE B.JumunNo = '%s' \n";
        $query = sprintf($query, $param);

        $rs = $conn->Execute($query);
        return $rs->fields["order_num"];
    }

    public function updateTypsetState($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query = "\n    UPDATE sheet_typset ";
        $query .= "\n   SET state = '%s' ";
        $query .= "\n   WHERE sheet_typset_seqno = %s ";
        if($param['state'] != '2420')
            $query .= "\n   AND state < '" . $param['state'] . "' ";

        $query = sprintf($query, $param["state"], $param["sheet_typset_seqno"]);
        $conn->Execute($query);

        $rs = $this->insertTypsetStateHistory($conn, $param);

        return $rs;
    }

    public function insertTypsetStateHistory($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param['date'] = "NOW()";
        if($param['state'] == '2680') {
            $param['date'] = "DATE_ADD(NOW(), INTERVAL 5 SECOND)";
        }

        $query = "\n    INSERT INTO typset_state_history ";
        $query .= "\n   (sheet_typset_seqno, state, state_modi_date, empl_id, detail) VALUES ";
        $query .= "\n   ('%s', '%s', %s, '%s', '%s') ";

        $query = sprintf($query, $param['sheet_typset_seqno'], $param['state'], $param['date'],$param['empl_id'],$param['detail']);
        return $conn->Execute($query);
    }

    public function selectedAllocatedOrder($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query =  " SELECT order_num FROM order_common \n";
        $query .= " WHERE order_mng = '%s' AND order_state = '1320' \n";

        $query = sprintf($query, $param['empl_id']);

        $rs = $conn->Execute($query);
        return $rs;
    }

    public function UpdateAutoWaitingOrder($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query =  " UPDATE order_common \n";
        $query .= " SET order_state = '1330', order_mng = '%s' \n";
        //$query .= " WHERE order_num = 'GPT220517NC02091' ";
        $query .= " WHERE receipt_dvs = 'Auto' AND (order_state = '1320' OR (order_state = '1330' AND order_mng = '%s'))\n";
        //$query .= " AND dlvr_dvs = 'leaflet' \n";
        $query .= " AND order_common_seqno >= 311915 \n";
        $query .= " Order By order_state DESC, order_regi_date ASC LIMIT 1 \n";

        $query = sprintf($query, $param['machine_id'],$param['machine_id']);

        $rs = $conn->Execute($query);
        return $rs;
    }

    public function updateOrderManager($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query =  " UPDATE order_common \n";
        $query .= " SET order_mng = '%s' \n";
        $query .= " WHERE order_num = '%s' AND order_state != '1180' \n";

        if($param['kind'] != "clear")
            $query .= " AND (order_mng = ''  OR order_mng IS NULL) \n";
        $query = sprintf($query, $param['empl_id'], $param['ordernum']);

        $conn->Execute($query);
        return $conn->affected_rows(true);
    }

     public function updateManager($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query =  " UPDATE member \n";
        $query .= " SET chk_yn = '%s' \n";
        $query .= " WHERE member_seqno = '%s' \n";

        $query = sprintf($query, $param['member'], $param['ordernum']);

        $conn->Execute($query);
        return $conn->affected_rows(true);
    }

    function selectOrderInfo($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n    SELECT  ";
        $query .= "\n            A.order_common_seqno";
        $query .= "\n           ,A.add_after_price";
        $query .= "\n           ,A.add_opt_price";
        $query .= "\n           ,A.grade_sale_price";
        $query .= "\n           ,A.member_sale_price";
        $query .= "\n           ,A.depo_price";
        $query .= "\n           ,A.member_seqno";
        $query .= "\n           ,A.card_depo_price";
        $query .= "\n           ,A.pay_way";
        $query .= "\n           ,A.order_lack_price";
        $query .= "\n           ,A.use_point_price";
        $query .= "\n           ,A.order_state";
        $query .= "\n           ,A.title";
        $query .= "\n           ,A.order_num";
        $query .= "\n           ,A.dlvr_dvs";
        $query .= "\n           ,B.deal_num";
        $query .= "\n           ,B.sell_price";
        $query .= "\n           ,B.dvs";
        $query .= "\n           ,B.pay_price";
        $query .= "\n           ,C.bun_dlvr_order_num";
        $query .= "\n      FROM  order_common AS A ";
        $query .= "\n      INNER JOIN member_pay_history AS B ON A.order_num = B.order_num ";
        $query .= "\n      INNER JOIN order_dlvr AS C ON C.order_common_seqno = A.order_common_seqno AND C.tsrs_dvs = '수신' ";
        $query .= "\n     WHERE  A.order_common_seqno = %s";

        $query  = sprintf($query, $param["order_seqno"]);

        $result = $conn->Execute($query);

        return $result;

    }

    function selectCountBunDelivery($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n    SELECT COUNT(*) cnt FROM order_dlvr AS A  ";
        $query .= "\n    INNER JOIN order_common AS B ON A.order_common_seqno = B.order_common_seqno ";
        $query .= "\n    WHERE A.bun_dlvr_order_num = %s AND B.member_seqno = %s AND A.tsrs_dvs = '수신' ";
        $query .= "\n    AND B.order_state != '1180' ";

        $query  = sprintf($query, $param["bun_dlvr_order_num"], $param["member_seqno"]);
        return $conn->Execute($query)->fields["cnt"];

    }

    function updateDeliveryFeeToAnotherRow($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n    UPDATE member_pay_history AS A   ";
        $query .= "\n    INNER JOIN member_pay_history AS B ON A.member_seqno = B.member_seqno AND A.dvs = B.dvs AND B.pay_price = 0 AND A.deal_date = B.deal_date ";
        $query .= "\n    INNER JOIN order_common AS C ON B.order_num = C.order_num AND C.dlvr_dvs = 'namecard' AND C.order_state NOT IN ('1080', '1120','1180','1220') AND C.OPI_Date IS NULL ";
        $query .= "\n    SET B.sell_price = A.sell_price, B.pay_price = A.pay_price, A.sell_price = 0, A.pay_price = 0 ";
        $query .= "\n    WHERE A.order_num = %s AND A.dvs = '배송비' AND A.pay_price != 0 ";
        $query .= "\n    LIMIT 1 ";

        $query  = sprintf($query, $param["order_num"]);
        $conn->Execute($query);
    }

    function selectBunDeliveryPrice($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n    SELECT MAX(B.sell_price) pay_price ";
        $query .= "\n      FROM  order_common AS A ";
        $query .= "\n      INNER JOIN member_pay_history AS B ON A.order_num = B.order_num ";
        $query .= "\n      INNER JOIN order_dlvr AS C ON C.order_common_seqno = A.order_common_seqno AND C.tsrs_dvs = '수신' ";
        $query .= "\n     WHERE  C.bun_dlvr_order_num = %s AND A.member_seqno = %s AND dvs = '배송비' ";

        $query  = sprintf($query, $param["bun_dlvr_order_num"], $param["member_seqno"]);
        return $conn->Execute($query)->fields["pay_price"];

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

    public function insertOrderTypeset($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query =  " INSERT INTO order_typset (typset_num, order_num, order_num_seq, amt) VALUES ('%s','%s',%s,%s)  \n";
        $query = sprintf($query, $param['typset_num'], $param['ordernum'], $param['seq'], $param['nestamt']);

        $rs = $conn->Execute($query);
        return $rs->fields;
    }

    public function deleteOrderTypeset($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query =  " DELETE FROM order_typset WHERE typset_num = '%s'  \n";
        $query = sprintf($query, $param['typset_num']);

        $rs = $conn->Execute($query);
        return $rs->fields;
    }

    public function selectProductStatecode($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query =  " SELECT A.order_state, A.cate_sortcode, B.cate_paper_mpcode FROM order_common AS A INNER JOIN order_detail AS B ON A.order_common_seqno = B.order_common_seqno WHERE A.order_num = '%s' \n";
        $query = sprintf($query, $param['ordernum']);

        $rs = $conn->Execute($query);
        return $rs->fields;
    }

    public function insertTypesetInfo($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query =  " INSERT INTO sheet_typset (typset_num, state, regi_date, \n";
        $query .= " beforeside_tmpt ,beforeside_spc_tmpt, aftside_tmpt, aftside_spc_tmpt, honggak_yn, print_amt, ";
        $query .= " print_amt_unit ,paper_name, prdt_page, memo, print_etprs, print_title, typset_way, dvs, size,dlvrboard, width, height, empl_seqno) ";  //13
        $query .= " VALUES ";
        $query .= " ('%s','%s',NOW(),'%s',0,'%s',0,'%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s',%s,%s, %s) "; //19
        $query = sprintf($query, $param['typset_num'] ,$param['state'],$param['beforeside_tmpt'],
            $param['aftside_tmpt'],$param['honggak_yn'],$param['print_amt'],$param['print_amt_dvs'],
            $param['paper_name'],$param['prdt_page'],$param['memo'], $param['print_etprs'] , $param['print_title'], $param['typset_way'] , $param['dvs'],$param['size'], $param['dlvrboard'], $param['width'], $param['height'], $param['empl_seqno'] );

        $rs = $conn->Execute($query);
        return $rs->fields;
    }

    public function insertTypesetInfo3($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query =  " INSERT INTO sheet_typset (typset_num, state, regi_date, \n";
        $query .= " beforeside_tmpt ,beforeside_spc_tmpt, aftside_tmpt, aftside_spc_tmpt, honggak_yn, print_amt, ";
        $query .= " print_amt_unit ,paper_name, prdt_page, memo, print_etprs, print_title, typset_way, dvs, size,dlvrboard) ";  //13
        $query .= " VALUES ";
        $query .= " ('%s','%s',NOW(),'%s',0,'%s',0,'%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s') "; //19
        $query = sprintf($query, $param['typset_num'] ,$param['state'],$param['beforeside_tmpt'],
            $param['aftside_tmpt'],$param['honggak_yn'],$param['print_amt'],$param['print_amt_dvs'],
            $param['paper_name'],$param['prdt_page'],$param['memo'], $param['print_etprs'] , $param['print_title'], $param['typset_way'] , $param['dvs'],$param['size'], $param['dlvrboard'] );

        $rs = $conn->Execute($query);
        return $rs->fields;
    }

    public function insertTypesetFile($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $file_path = explode('-', $param['typset_num_file']);
        $query =  " INSERT INTO sheet_typset_file (file_path, save_file_name, origin_file_name, size, sheet_typset_seqno) \n";
        $query .= " VALUES ";
        $query .= " ('%s','%s', '%s','%s','%s') "; //13
        $query = sprintf($query,
            "/attach/gp/typset_file/" . $file_path[0] . "-" . $file_path[1] . "/" ,
            $param['typset_num_file'] . ".pdf",
            $param['typset_num_file'] . ".pdf",
            "0",
            $param['sheet_typset_seqno']);

        $rs = $conn->Execute($query);
        return $rs->fields;
    }

    public function InitDeliveryInfo($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query =  " UPDATE order_dlvr AS A  \n";
        $query .= " INNER JOIN order_common B ON A.order_common_seqno = B.order_common_seqno \n";
        $query .= " INNER JOIN order_dlvr C ON A.bun_dlvr_order_num = C.bun_dlvr_order_num AND A.name = C.name  \n";
        $query .= " SET C.invo_num = '' \n";
        $query .= " WHERE B.order_num = '%s' \n";
        $query = sprintf($query, $param['ordernum']);

        $rs = $conn->Execute($query);
        return $rs->fields;
    }

    public function UpdateDeliveryInfo($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query =  " UPDATE order_dlvr AS A  \n";
        $query .= " INNER JOIN order_common B ON A.order_common_seqno = B.order_common_seqno \n";
        $query .= " INNER JOIN order_dlvr C ON A.bun_dlvr_order_num = C.bun_dlvr_order_num AND A.name = C.name  \n";
        $query .= " SET C.invo_num = TRIM(',' FROM CONCAT('%s', ',',C.invo_num)) \n";
        $query .= " WHERE B.order_num = '%s' \n";
        $query = sprintf($query, $param['invo_num'], $param['ordernum']);

        $rs = $conn->Execute($query);
        return $rs->fields;
    }

    public function insertTypesetLabel($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query =  " INSERT INTO sheet_typset_label_file (file_path, save_file_name, origin_file_name, size, sheet_typset_seqno) \n";
        $query .= " VALUES ";
        $query .= " ('%s','%s', '%s','%s','%s') "; //13
        $query = sprintf($query,
            "/attach/gp/label_file/" . $param['typset_num_file'] . "/" ,
            $param['typset_num_file'] . ".pdf",
            $param['typset_num_file'] . ".pdf",
            "0",
            $param['sheet_typset_seqno']);
        $rs = $conn->Execute($query);
        return $rs->fields;
    }

    public function deleteTypesetInfo($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query =  " DELETE FROM sheet_typset WHERE typset_num = '%s'  \n";
        $query = sprintf($query, $param['typset_num']);

        $rs = $conn->Execute($query);
        return $rs->fields;
    }

    public function deleteTypesetFile($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query =  " DELETE A FROM sheet_typset_file AS A \n";
        $query .=  " INNER JOIN sheet_typset AS B ON A.sheet_typset_seqno = B.sheet_typset_seqno  \n";
        $query .=  " WHERE B.typset_num = '%s'  \n";
        $query = sprintf($query, $param['typset_num']);

        $rs = $conn->Execute($query);
        return $rs->fields;
    }

    public function deleteTypesetLabel($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query =  " DELETE A FROM sheet_typset_label_file AS A  \n";
        $query .=  " INNER JOIN sheet_typset AS B ON A.sheet_typset_seqno = B.sheet_typset_seqno  \n";
        $query .=  " WHERE B.typset_num = '%s'  \n";
        $query = sprintf($query, $param['typset_num']);

        $rs = $conn->Execute($query);
        return $rs->fields;
    }

    public function selectAdminAccount($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query =  " SELECT * from empl where empl_id = '%s'   \n";
        $query = sprintf($query, $param['state'], $param['state'], $param['accept_file_path'],
            $param['accept_file_name'], $param['preview_file_path'],
            $param['preview_file_name'], $param['ordernum']);

        $rs = $conn->Execute($query);
        return $rs->fields;
    }

    public function selectProductList($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query = "\n    select MAX(A.order_detail_file_num) as order_detail_file_num, MAX(A.print_file_path) as print_file_path, ";
        $query .= "\n   MAX(A.print_file_name) as print_file_name, MAX(B.work_size_wid) as work_size_wid, MAX(B.amt) as amt, MAX(B.amt_unit_dvs) as amt_unit_dvs, ";
        $query .= "\n   MAX(E.oper_sys) as oper_sys, MAX(E.title) as title, MAX(E.order_detail) as order_detail, MAX(E.order_regi_date) as order_regi_date,  ";
        $query .= "\n   MAX(E.pay_price) as price, MAX(G.cate_name) as cate_name, MAX(B.tot_tmpt) as tot_tmpt, MAX(E.count) as count, MAX(E.order_mng) as order_mng, MAX(F.member_name) as send_name,";
        $query .= "\n   MAX(D.name) as receive_name, MAX(D.tel_num) as receive_telnum, MAX(D.cell_num) as receive_cellnum, MAX(D.addr) as receive_addr, MAX(D.addr_detail) as receive_addr_detail, MAX(D.dlvr_way) as dlvr_way, MAX(D.dlvr_sum_way) as dlvr_sum_way,  ";
        $query .= "\n   MAX(B.work_size_vert) as work_size_vert, MAX(B.cut_size_wid) as cut_size_wid, MAX(B.cut_size_vert) as cut_size_vert,  MAX(D.dlvr_way) as dlvr_way, MAX(D.dlvr_sum_way) as dlvr_sum_way, MAX(F.member_name) as member_name, ";
        $query .= "\n   MAX(C.preview_file_path) as preview_file_path, GROUP_CONCAT(C.preview_file_name Order by C.preview_file_name SEPARATOR '|') as preview_file_name ";
        $query .= "\n   from order_detail_count_file AS A  ";
        $query .= "\n   INNER JOIN order_detail AS B ON A.order_detail_seqno = B.order_detail_seqno  ";
        $query .= "\n   INNER JOIN order_detail_count_preview_file AS C ON A.order_detail_count_file_seqno = C.order_detail_count_file_seqno  ";
        $query .= "\n   INNER JOIN order_dlvr AS D ON D.order_common_seqno = B.order_common_seqno AND D.tsrs_dvs = '수신'  ";
        $query .= "\n   INNER JOIN order_common AS E ON E.order_common_seqno = B.order_common_seqno ";
        $query .= "\n   INNER JOIN member AS F ON F.member_seqno = E.member_seqno ";
        $query .= "\n   INNER JOIN cate AS G ON E.cate_sortcode = G.sortcode ";
        if($param['sortcode'] == "003001001" || $param['sortcode'] == "003001003")
            $query .= "\n   where B.cate_sortcode IN ('003001001','003001003') and E.amt = %s and B.tot_tmpt = %s ";
        else if($param['sortcode'] == "003001002" || $param['sortcode'] == "003001004")
            $query .= "\n   where B.cate_sortcode IN ('003001002','003001004') and E.amt = %s and B.tot_tmpt = %s ";
        else if(substr($param['sortcode'], 0, 6) === "003002")
            $query .= "\n   where B.cate_sortcode = '%s' and B.cate_paper_mpcode = %s and B.tot_tmpt = %s ";
        else
            $query .= "\n   where B.cate_sortcode = '%s' and B.cate_paper_mpcode = %s and E.amt = %s and B.tot_tmpt = %s ";
        $query .= "\n   AND A.state = '2120' AND C.order_detail_count_file_seqno is not null ";
        $query .= "\n   group by A.order_detail_file_num  ";

        if($param['sortcode'] == "003001001" || $param['sortcode'] == "003001002" || $param['sortcode'] == "003001003" || $param['sortcode'] == "003001004")
            $query = sprintf($query, $param['amt'], $param['side']);
        if($param['sortcode'] == "003002001")
            $query = sprintf($query, $param["sortcode"], $param["mpcode"], $param['side']);
        else
            $query = sprintf($query, $param["sortcode"], $param["mpcode"], $param['amt'], $param['side']);

        $rs = $conn->Execute($query);
        $products = array();
        while ($rs && !$rs->EOF) {
            $product = array();
            $product["order_detail_file_num"] = $rs->fields["order_detail_file_num"];
            $product["print_file_path"] = $rs->fields["print_file_path"];
            $product["print_file_name"] = $rs->fields["print_file_name"];
            $product["preview_file_path"] = $rs->fields["preview_file_path"];
            $product["preview_file_name"] = $rs->fields["preview_file_name"];
            $product["member_name"] = $rs->fields["member_name"];
            $product["work_size_wid"] = $rs->fields["work_size_wid"];
            $product["work_size_vert"] = $rs->fields["work_size_vert"];
            $product["cut_size_wid"] = $rs->fields["cut_size_wid"];
            $product["cut_size_vert"] = $rs->fields["cut_size_vert"];
            $product["amt"] = $rs->fields["amt"];
            $product["amt_unit_dvs"] = $rs->fields["amt_unit_dvs"];
            $product["dlvr_way"] = $rs->fields["dlvr_way"];
            $product["dlvr_sum_way"] = $rs->fields["dlvr_sum_way"];
            $product["filenames"] = $rs->fields["preview_file_name"];

            $product["oper_sys"] = $rs->fields["oper_sys"];
            $product["title"] = $rs->fields["title"];
            $product["order_detail"] = $rs->fields["order_detail"];
            $product["order_regi_date"] = $rs->fields["order_regi_date"];
            $product["price"] = $rs->fields["price"];
            $product["cate_name"] = $rs->fields["cate_name"];
            $product["tot_tmpt"] = $rs->fields["tot_tmpt"];
            $product["count"] = $rs->fields["count"];
            $product["receive_name"] = $rs->fields["receive_name"];
            $product["receive_telnum"] = $rs->fields["receive_telnum"];
            $product["receive_cellnum"] = $rs->fields["receive_cellnum"];
            $product["receive_addr"] = $rs->fields["receive_addr"];
            $product["receive_addr_detail"] = $rs->fields["receive_addr_detail"];
            $product["send_name"] = $rs->fields["send_name"];
            $product["dlvr_way"] = $rs->fields["dlvr_way"];
            $product["dlvr_sum_way"] = $rs->fields["dlvr_sum_way"];

            array_push($products, $product);
            $rs->MoveNext();
        }

        return $products;
    }

    public function selectAfterInfo() {

    }

    public function  selectOrderFile($conn, $param) {
        $query = "\n    select * ";
        $query .= "\n   from order_detail_count_preview_file ";
        $query .= "\n   WHERE preview_file_name = '%s' ";

        $query = sprintf($query, $param["order_detail_count_preview_file"]);

        $rs = $conn->Execute($query);
        $file_info = array();
        while ($rs && !$rs->EOF) {
            return $rs->fields["preview_file_path"] . "/" . $rs->fields["preview_file_name"];
        }

        return null;
    }

    public function selectPDFPath($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query = "\n   SELECT * ";
        $query .= "\n   FROM order_detail_count_file ";
        $query .= "\n   WHERE order_detail_file_num = %s ";

        $query = sprintf($query, $param["order_detail_file_num"]);
        $rs = $conn->Execute($query);
        $arr = array();
        while ($rs && !$rs->EOF) {
            return $rs->fields["print_file_path"] . "/" . $rs->fields["print_file_name"];
        }
    }

    public function selectAllPapers($conn) {
        $query = "\n   select concat(name, \" \", dvs, \" \", color, \" \", basisweight) as name, cate_paper_seqno  from cate_paper ";
        $query .= "\n  group by name, dvs, color, basisweight ";

        $rs = $conn->Execute($query);
        $names = array();
        while($rs && !$rs->EOF) {
            array_push($names, $rs->fields["name"]);
            $rs->MoveNext();
        }

        return $names;
    }

    public function selectPathList($conn, $param)
    {
        $query = "\n    select B.cate_sortcode, C.tot_name, C.cate_name, D.name, D.color, D.basisweight, D.mpcode, B.tot_tmpt,B.amt ,Count(*) as cnt  ";
        $query .= "\n   from order_detail_count_file AS A  ";
        $query .= "\n   INNER JOIN order_detail AS B ON A.order_detail_seqno = B.order_detail_seqno ";
        $query .= "\n   INNER JOIN cate AS C ON B.cate_sortcode = C.sortcode ";
        $query .= "\n   INNER JOIN cate_paper AS D ON D.mpcode = B.cate_paper_mpcode ";
        $query .= "\n   where A.state = '2120' AND B.receipt_finish_date BETWEEN '%s' AND '%s 23:59:59' ";
        $query .= "\n   group by B.cate_sortcode, D.name, D.color, D.basisweight, D.mpcode, B.tot_tmpt, B.amt ";
        $query .= "\n   order by B.cate_sortcode ";

        $query = sprintf($query, $param['date'], $param['date']);
        $rs = $conn->Execute($query);
        $file_list = array();
        $file_list_count = array();
        $i = 0;
        while ($rs && !$rs->EOF) {
            // 코팅명함
            if ($rs->fields["cate_sortcode"] == "003001001" || $rs->fields["cate_sortcode"] == "003001003") {
                $small_cate = $rs->fields["tot_tmpt"] . "_" . $rs->fields["amt"];

                if (!isset($file_list_count["(명함)일반명함"]["코팅명함"][$small_cate])) {
                    $file_list[$i]["sortcode"] = $rs->fields["cate_sortcode"];
                    $file_list[$i]["mpcode"] = $rs->fields["mpcode"];
                    $file_list[$i]["large_category"] = "(명함)일반명함";
                    $file_list[$i]["middle_category"] = "코팅명함";
                    $file_list[$i++]["small_category"] = $small_cate;
                }

                $file_list_count["(명함)일반명함"]["코팅명함"][$small_cate] += $rs->fields["cnt"];
            } // 무코팅명함
            else if ($rs->fields["cate_sortcode"] == "003001002" || $rs->fields["cate_sortcode"] == "003001004") {
                $small_cate = $rs->fields["tot_tmpt"] . "_" . $rs->fields["amt"];

                if (!isset($file_list_count["(명함)일반명함"]["무코팅명함"][$small_cate])) {
                    $file_list[$i]["sortcode"] = $rs->fields["cate_sortcode"];
                    $file_list[$i]["mpcode"] = $rs->fields["mpcode"];
                    $file_list[$i]["large_category"] = "(명함)일반명함";
                    $file_list[$i]["middle_category"] = "무코팅명함";
                    $file_list[$i++]["small_category"] = $small_cate;
                }

                $file_list_count["(명함)일반명함"]["무코팅명함"][$small_cate] += $rs->fields["cnt"];
            } // 고급명함
            else if ($rs->fields["cate_sortcode"] == "003002001") {
                $nc_name = $rs->fields["name"] . " " . $rs->fields["color"] . " " . $rs->fields["basisweight"];
                $small_cate = $rs->fields["tot_tmpt"] . "_" . "200";

                if (!isset($file_list_count["(명함)고급명함"][$nc_name][$small_cate])) {
                    $file_list[$i]["sortcode"] = $rs->fields["cate_sortcode"];
                    $file_list[$i]["mpcode"] = $rs->fields["mpcode"];
                    $file_list[$i]["large_category"] = "(명함)고급명함";
                    $file_list[$i]["middle_category"] = $nc_name;
                    $file_list[$i++]["small_category"] = $small_cate;
                }

                $file_list_count["(명함)고급명함"][$nc_name][$small_cate] += $rs->fields["cnt"];
            } // 강접스티커
            else if ($rs->fields["cate_sortcode"] == "004001001") {
                $small_cate = $rs->fields["tot_tmpt"] . "_" . $rs->fields["amt"];

                if (!isset($file_list_count["(스티커)컬러스티커"]["강접스티커"][$small_cate])) {
                    $file_list[$i]["sortcode"] = $rs->fields["cate_sortcode"];
                    $file_list[$i]["mpcode"] = $rs->fields["mpcode"];
                    $file_list[$i]["large_category"] = "(스티커)컬러스티커";
                    $file_list[$i]["middle_category"] = "강접스티커";
                    $file_list[$i++]["small_category"] = $small_cate;
                }

                $file_list_count["(스티커)컬러스티커"]["강접스티커"][$small_cate] += $rs->fields["cnt"];
            } // 특수지스티커
            else if ($rs->fields["cate_sortcode"] == "004002001") {
                $st_name = $rs->fields["name"] . "스티커";
                $small_cate = $rs->fields["tot_tmpt"] . "_" . $rs->fields["amt"];

                if (!isset($file_list_count["(스티커)특수지스티커"][$st_name][$small_cate])) {
                    $file_list[$i]["sortcode"] = $rs->fields["cate_sortcode"];
                    $file_list[$i]["mpcode"] = $rs->fields["mpcode"];
                    $file_list[$i]["large_category"] = "(스티커)특수지스티커";
                    $file_list[$i]["middle_category"] = $st_name;
                    $file_list[$i++]["small_category"] = $small_cate;
                }

                $file_list_count["(스티커)특수지스티커"][$st_name][$small_cate] += $rs->fields["cnt"];
            } // 도무송스티커
            else if ($rs->fields["cate_sortcode"] == "004003001") {
                $st_name = $rs->fields["name"] . "스티커";
                $small_cate = $rs->fields["tot_tmpt"] . "_" . $rs->fields["amt"];

                if (!isset($file_list_count["(스티커)도무송스티커"][$st_name][$small_cate])) {
                    $file_list[$i]["sortcode"] = $rs->fields["cate_sortcode"];
                    $file_list[$i]["mpcode"] = $rs->fields["mpcode"];
                    $file_list[$i]["large_category"] = "(스티커)도무송스티커";
                    $file_list[$i]["middle_category"] = $st_name;
                    $file_list[$i++]["small_category"] = $small_cate;
                }

                $file_list_count["(스티커)도무송스티커"][$st_name][$small_cate] += $rs->fields["cnt"];
            } // 합판전단지
            else if ($rs->fields["cate_sortcode"] == "005001001") {
                $small_cate = $rs->fields["tot_tmpt"] . "_" . $rs->fields["amt"];

                if (!isset($file_list_count["(전단)합판전단지"]["아트지 90g"][$small_cate])) {
                    $file_list[$i]["sortcode"] = $rs->fields["cate_sortcode"];
                    $file_list[$i]["mpcode"] = $rs->fields["mpcode"];
                    $file_list[$i]["large_category"] = "(전단)합판전단지";
                    $file_list[$i]["middle_category"] = "아트지 90g";
                    $file_list[$i++]["small_category"] = $small_cate;
                }

                $file_list_count["(전단)합판전단지"]["아트지 90g"][$small_cate] += $rs->fields["cnt"];
            }

            $rs->MoveNext();
        }

        $i = 0;
        $rs_arr = array();
        foreach ($file_list as $file) {
            //$file_list[$i]["count"] = $file_list_count[$file["large_category"]][$file["middle_category"]][$file["small_category"]];
            $afile = array();
            $afile["mpcode"] = $file["mpcode"];
            $afile["sortcode"] = $file["sortcode"];
            $afile["large_category"] = $file["large_category"];
            $afile["middle_category"] = $file["middle_category"];
            $afile["small_category"] = $file["small_category"];
            $afile["count"] = $file_list_count[$file["large_category"]][$file["middle_category"]][$file["small_category"]];
            array_push($rs_arr, $afile);
        }

        return $rs_arr;
    }

    function startsWith($haystack, $needle)
    {
        $length = strlen($needle);
        return (substr($haystack, 0, $length) === $needle);
    }
}
