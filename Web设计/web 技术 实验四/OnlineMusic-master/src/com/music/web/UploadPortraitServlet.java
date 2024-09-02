package com.music.web;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.sql.SQLException;
import java.util.UUID;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import com.music.service.UpdateCommentPortraitService;
import com.music.service.UploadPortraitService;
import sun.misc.BASE64Decoder;
import sun.misc.BASE64Encoder;

public class UploadPortraitServlet extends HttpServlet {

	protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		String portraitUrl = request.getParameter("head_imgurl");
		String userName = request.getParameter("userName");
		String imgurl = "images/serverImage/"+UUID.randomUUID().toString()+".jpg";
		boolean flags = GenerateImage(portraitUrl,"/Library/tomcat/webapps/music/"+imgurl);
		System.out.println(imgurl);
		System.out.println("flags:"+flags);
		System.out.println(flags == true);
		UploadPortraitService service = new UploadPortraitService();
		String flag = null;
		if (flags == true) {
			try {
				flag = service.upload(imgurl,userName);
				System.out.println("flag:"+flag);
				if (flag.equals("true")) {
					// 同步修改评论列表的头像
					UpdateCommentPortraitService service1 = new UpdateCommentPortraitService();
					String flag1 = null;
					try {
						flag1 = service1.updateCommentPortrait(userName,imgurl);
					} catch (NumberFormatException | SQLException e) {
						e.printStackTrace();
					}
				}
			} catch (SQLException e) {
				e.printStackTrace();
			}
		} else {
			flag = "false";
		}
		response.getWriter().write(flag);
	}
	
	
	public static boolean GenerateImage(String imgStr, String imgFilePath) {// 对字节数组字符串进行Base64解码并生成图片
        if (imgStr == null) // 图像数据为空
            return false;
        BASE64Decoder decoder = new BASE64Decoder();
        try {
            // Base64解码
            byte[] bytes = decoder.decodeBuffer(imgStr);
            for (int i = 0; i < bytes.length; ++i) {
                if (bytes[i] < 0) {// 调整异常数据
                    bytes[i] += 256;
                }
            }
            // 生成图片
            OutputStream out = new FileOutputStream(imgFilePath);
            out.write(bytes);
            out.flush();
            out.close();
            return true;
        } catch (Exception e) {
            return false;
        }
	}
	
//	public String parseContents(String contents) {
//	    //String newContents=contents;
//	    if (contents.indexOf("data:image/") != -1) {
//	        int firstIndex = contents.indexOf("data:image/") + 11;
//	        int index1 = contents.indexOf(";base64,");
//	        String type = contents.substring(firstIndex, index1);
//	        String fileName = "E:/uploadFiles/" + UUID.randomUUID().toString() + "." + type;
//	        //开始转码，然后将当前文件写入数据。
//	        System.out.println(fileName);
//	        BASE64Decoder decoder = new BASE64Decoder();
//	        OutputStream os = null;
//	        try {
//	            String imgsrc = StringUtils.substringBefore(contents.substring(contents.indexOf(";base64,") + 8), "\"");
//	            byte[] bytes = decoder.decodeBuffer(imgsrc);
//	            //替换之前的src中base64数据为servlet请求
//	            contents = contents.replace("data:image/" + type + ";base64," + imgsrc, "/images.do?src=" + fileName);
//	            File file = new File(fileName);
//	            //获取父目录
//	            File fileParent = file.getParentFile();
//	            //判断是否存在
//	            if (!fileParent.exists()) {
//	                //创建父目录文件
//	                fileParent.mkdirs();
//	            }
//	            file.createNewFile();
//	            os = new FileOutputStream(file);
//	            os.write(bytes);
//
//	        } catch (Exception e) {
//	            e.printStackTrace();
//	        } finally {
//	            if (os != null) {
//	                try {
//	                    os.close();
//	                } catch (IOException e) {
//	                    e.printStackTrace();
//	                }
//	            }
//	        }
//	    }
//	    return contents;
//	}
	

	protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		doGet(request, response);
	}

}