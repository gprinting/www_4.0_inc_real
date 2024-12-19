<?
include_once(INC_PATH . "/com/nexmotion/job/nimda/common/BusinessCommonDAO.inc");
include_once(INC_PATH . "/com/nexmotion/html/nimda/business/oto_inq_mng/OtoInqList.inc");

/**
 * @file OtoInqMngDAO.inc
 *
 * @brief 영업 - 1:1문의관리 - 1대1문의리스트 DAO
 */
class OtoInqMngDAO extends BusinessCommonDAO {

    function __construct() {
    }

    /**
     * @brief 1:1문의리스트
     *
     * @param $conn = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     *
    function selectOtoInquireList($conn, $param) {

        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);
        $dvs = substr($param["dvs"], 1, -1);

        if ($dvs == "COUNT") {
            $query  ="\nSELECT  COUNT(*) AS cnt ";
        } else {
            $query  = "\nSELECT  T1.* ";
            $query .= "\n       ,T2.office_nick ";
            $query .= "\n       ,T2.member_name ";
        }
        $query .= "\n  FROM  ( ";
        $query .= "\n           SELECT  A.regi_date AS inq_date ";
        $query .= "\n                  ,A.member_seqno ";
        $query .= "\n                  ,A.inq_typ ";
        $query .= "\n                  ,A.title ";
        $query .= "\n                  ,B.regi_date AS reply_date ";
        $query .= "\n                  ,C.name ";
        $query .= "\n                  ,A.answ_yn ";
        $query .= "\n                  ,A.oto_inq_seqno ";
        $query .= "\n            FROM  oto_inq AS A ";
        $query .= "\n       LEFT JOIN  oto_inq_reply AS B ";
        $query .= "\n              ON  A.oto_inq_seqno = B.oto_inq_seqno ";
        $query .= "\n       LEFT JOIN  empl AS C ";
        $query .= "\n              ON  B.empl_seqno = C.empl_seqno ) AS T1 ";
        $query .= "\n         ,member AS T2 ";
        $query .= "\n WHERE  T1.member_seqno = T2.member_seqno ";

        //판매채널
        if ($this->blankParameterCheck($param ,"sell_site")) {
            $query .="\n    AND  T2.cpn_admin_seqno = $param[sell_site] ";
        }
        //팀
        if ($this->blankParameterCheck($param ,"depar_code")) {
            $query .="\n    AND  T2.biz_resp = $param[depar_code] ";
        }
        //사내닉네임(회원명) -> 일련번호로 변형 해서 검색
        if ($this->blankParameterCheck($param ,"member_seqno")) {
            $query .="\n    AND  T2.member_seqno = $param[member_seqno] ";
        }
        //문의일, 답변일
        if ($this->blankParameterCheck($param ,"from")) {
            $val = substr($param["search_cnd"], 1, -1);
            $query .="\n   AND  T1.$val >= $param[from] ";
        }
        if ($this->blankParameterCheck($param ,"to")) {
            $val = substr($param["search_cnd"], 1, -1);
            $query .="\n   AND  T1.$val <= $param[to] ";
        }
        //상태
        if ($this->blankParameterCheck($param ,"answ_yn")) {
            $query .="\n   AND  T1.answ_yn = $param[answ_yn] ";
        }

        $query .="\n ORDER BY T1.oto_inq_seqno DESC ";

        $s_num = substr($param["s_num"], 1, -1);
        $list_num = substr($param["list_num"], 1, -1);

        if (!$dvs) {
            $query .= "\nLIMIT ". $s_num . ", " . $list_num;
        }

        return $conn->Execute($query);
    }*/

    /**
     * @brief 1:1문의리스트
     *
     * @param $conn = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectOtoInquireList($conn, $param, $page_count) {

        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $excpt_arr  = array("empl_seqno" => true);
        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param, $excpt_arr);

        $query  = "\nSELECT  /* 영업-1:1문의 검색용 쿼리 */ ";
        if (!$this->blankParameterCheck($param, "page_dvs")) {
            $query .= "\n         SQL_CALC_FOUND_ROWS";
        }
        $query .= "\n        T1.* ";
        $query .= "\n       ,T2.office_nick ";
        $query .= "\n       ,T2.member_name ";
        $query .= "\n  FROM  ( ";
        $query .= "\n           SELECT  A.regi_date AS inq_date ";
        $query .= "\n                  ,A.member_seqno ";
        $query .= "\n                  ,A.inq_typ ";
        $query .= "\n                  ,A.title ";
        $query .= "\n                  ,B.regi_date AS reply_date ";
        $query .= "\n                  ,D.name ";
        $query .= "\n                  ,A.answ_yn ";
        $query .= "\n                  ,A.oto_inq_seqno ";
        $query .= "\n                  ,B.empl_seqno ";
        $query .= "\n            FROM  oto_inq AS A ";
        $query .= "\n       LEFT JOIN  oto_inq_reply AS B ";
        $query .= "\n              ON  A.oto_inq_seqno = B.oto_inq_seqno AND B.is_deleted = 'N' ";
        $query .= "\n       LEFT JOIN  empl AS C ";
        $query .= "\n              ON  B.empl_seqno = C.empl_seqno LEFT JOIN empl_human_info AS D ON C.empl_seqno = D.empl_seqno ) AS T1 ";
        $query .= "\n         ,member AS T2 ";
        if ($this->blankParameterCheck($param ,"empl_seqno")) {
            $query .= "\n         ,empl AS T3 ";
        }
        $query .= "\n WHERE  T1.member_seqno = T2.member_seqno ";

        //판매채널
        if ($this->blankParameterCheck($param ,"sell_site")) {
            //$query .="\n    AND  T2.cpn_admin_seqno = $param[sell_site] ";
        }
        //팀
        if ($this->blankParameterCheck($param ,"empl_seqno")) {
            $query .="\n    AND  T1.empl_seqno = T3.empl_seqno ";
            $query .="\n    AND  T3.empl_seqno IN ($param[empl_seqno]) ";
        }
        //사내닉네임(회원명) -> 일련번호로 변형 해서 검색
        if ($this->blankParameterCheck($param ,"member_seqno")) {
            $query .="\n    AND  T2.member_seqno = $param[member_seqno] ";
        }
        //문의일, 답변일
        if ($this->blankParameterCheck($param ,"from")) {
            $val = substr($param["date_sel"], 1, -1);
            $query .="\n   AND  T1.$val >= ";
            $query .= substr($param[from], 0, -1) . " 00:00:00'";
        }
        if ($this->blankParameterCheck($param ,"to")) {
            $val = substr($param["date_sel"], 1, -1);
            $query .="\n   AND  T1.$val <= ";
            $query .= substr($param[to], 0, -1) . " 23:59:59'";
        }
        //상태
        if ($this->blankParameterCheck($param ,"answ_yn")) {
            $query .="\n   AND  T1.answ_yn = $param[answ_yn] ";
        }

        $query .="\n ORDER BY T1.oto_inq_seqno DESC ";
        $query .= "\nLIMIT ". $page_count ." ,  5";

        return $conn->Execute($query);
    }

    /**
     * @brief 문의내용
     *
     * @param $conn = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectOtoInqCont($conn, $param) {

        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\nSELECT  A.title ";
        $query .= "\n       ,A.inq_typ ";
        $query .= "\n       ,B.member_name ";
        $query .= "\n       ,B.office_nick ";
        $query .= "\n       ,A.tel_num ";
        $query .= "\n       ,A.cell_num ";
        $query .= "\n       ,A.mail ";
        $query .= "\n       ,A.cont ";
        $query .= "\n       ,A.answ_yn ";
        $query .= "\n  FROM  oto_inq AS A ";
        $query .= "\n       ,member AS B ";
        $query .= "\n WHERE  A.member_seqno = B.member_seqno ";

        //일련번호
        if ($this->blankParameterCheck($param ,"oto_inq_seqno")) {
            $query .="\n    AND A.oto_inq_seqno = $param[oto_inq_seqno] ";
        }

        return $conn->Execute($query);
    }

    /**
     * @brief 답변내용
     *
     * @param $conn = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectOtoReplyCont($conn, $param) {

        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\nSELECT C.name,A.cont,A.oto_inq_reply_seqno ";
        $query .= "\n       FROM oto_inq_reply AS A ";
        $query .= "\n       INNER JOIN empl AS B ON A.empl_seqno = B.empl_seqno ";
        $query .= "\n       INNER JOIN empl_human_info AS C ON A.empl_seqno = C.empl_seqno ";
        $query .= "\n  WHERE 1=1 AND is_deleted = 'N' ";

        //일련번호
        if ($this->blankParameterCheck($param ,"oto_inq_seqno")) {
            $query .="\n    AND A.oto_inq_seqno = $param[oto_inq_seqno] ";
        }

        return $conn->Execute($query);
    }
}
