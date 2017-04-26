#pragma once



#ifndef U_SIFT_H
#define U_SIFT_H


namespace MySift {
	class UnpackedSIFT {

	public:
		UnpackedSIFT();
		UnpackedSIFT(int _nfeatures, int _nOctaveLayers, double _contrastThreshold, double _edgeThreshold, double _sigma);

		int descriptorSize() const;
		int descriptorType() const;

		void operator()(cv::InputArray _image, cv::InputArray _mask, std::vector<cv::KeyPoint>& keypoints) const;


		void operator()(cv::InputArray _image, cv::InputArray _mask, std::vector<cv::KeyPoint>& keypoints, cv::OutputArray _descriptors, bool useProvidedKeypoints) const;

		void detectImpl(const cv::Mat & image, std::vector<cv::KeyPoint>& keypoints, const cv::Mat & mask) const;

		void computeImpl(const cv::Mat & image, std::vector<cv::KeyPoint>& keypoints, cv::Mat & descriptors) const;

		static cv::Ptr<UnpackedSIFT> create(int nfeatures = 0, int nOctaveLayers = 3, double contrastThreshold = 0.04, double edgeThreshold = 10, double sigma = 1.6);

	private:
		int nfeatures = 0;
		int nOctaveLayers = 3;
		double contrastThreshold = 0.04;
		double edgeThreshold = 10;
		double sigma = 1.6;
		bool showProgression = false;
		bool progressionPause = false;

		void findScaleSpaceExtrema(const std::vector<cv::Mat>& gauss_pyr, const std::vector<cv::Mat>& dog_pyr, std::vector<cv::KeyPoint>& keypoints) const;

		void buildGaussianPyramid(const cv::Mat & base, std::vector<cv::Mat>& pyr, int nOctaves) const;

		void buildDoGPyramid(const std::vector<cv::Mat>& gpyr, std::vector<cv::Mat>& dogpyr) const;

	};
}

#endif // !U_SIFT_H