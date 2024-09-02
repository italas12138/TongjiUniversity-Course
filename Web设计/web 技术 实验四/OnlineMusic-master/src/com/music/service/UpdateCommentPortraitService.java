package com.music.service;

import java.sql.SQLException;

import com.music.dao.UpdateCommentPortraitdao;

public class UpdateCommentPortraitService {

	public String updateCommentPortrait(String userName, String imgurl) throws NumberFormatException, SQLException {
		UpdateCommentPortraitdao dao = new UpdateCommentPortraitdao();
		String flag = dao.updateCommentPortrait(userName,imgurl);
		return flag;
	}

}
