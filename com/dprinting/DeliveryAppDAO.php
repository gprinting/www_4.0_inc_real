<?php
/**
 * Created by PhpStorm.
 * User: Hyeonsik Cho
 * Date: 2017-09-04
 * Time: 오후 6:14
 * 배송관리 어플리케이션용 DAO
 */

include_once(INC_PATH . '/com/nexmotion/job/nimda/common/NimdaCommonDAO.inc');
include_once(INC_PATH . "/common_lib/cypress_file.inc");

$curDirectory = dirname(__FILE__);
define("LOGPATH", $curDirectory . "/logs/" . date("Y_m_d"));

class DeliveryAppDAO extends NimdaCommonDAO
{
    function __construct()
    {
    }

    public function selectDeliveryList($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query = "\n    SELECT C.addr, C.addr_detail, C.tel_num, C.cell_num, C.name, A.order_detail, A.amt ";
        $query .= "\n   FROM order_common AS A ";
        $query .= "\n   INNER JOIN member AS B ON A.member_seqno = B.member_seqno ";
        $query .= "\n   INNER JOIN order_dlvr AS C ON A.order_common_seqno = C.order_common_seqno AND C.tsrs_dvs = '수신' ";
        $query .= "\n   INNER JOIN order_detail AS D ON A.order_common_seqno = D.order_common_seqno ";
        $query .= "\n   INNER JOIN order_detail AS D ON A.order_common_seqno = D.order_common_seqno ";

        if($param['Istodaydelivery'] == "Y") {
            $query .= "\n   INNER JOIN order_opt_history AS E ON E.order_common_seqno = A.order_common_seqno AND E.opt_name = '당일판'";
        }

        $query .= "\n  WHERE 1 = 1 ";
        $query .= "\n  AND A.title like %%%s%%";
        $query .= "\n  AND C.name like %%%s%% ";
        $query .= "\n  AND A.order_regi_date BETWEEN %s AND %s 23:59:59 ";
        $query .= "\n  AND C.dlvr_way = %s ";
        $query .= "\n  AND D.after_use_yn = %s ";


        $query = sprintf($query
            ,$param['title']
            ,$param['membername']
            ,$param['startdate']
            ,$param['enddate']
            ,$param['deliveryway']
            ,$param['isincludedafterprocess']);

        echo $query;
        exit;
        $rs = $conn->Execute($query);


    }
}