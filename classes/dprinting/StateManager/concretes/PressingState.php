<?php
/**
 * Created by PhpStorm.
 * User: Hyeonsik Cho
 * Date: 2017-10-27
 * Time: 오후 6:12
 * desc: 조판중
 */

include_once(INC_PATH . "/classes/dprinting/StateManager/abstracts/AbstractOrderByCountState.php");

class PressingState extends AbstractOrderByCountState
{
    public function ToNextState()
    {
        //
        parent::ToNextState();
    }
}

?>