#pragma once



#ifndef U_SIFT_H
#define U_SIFT_H


namespace MySift {
	/// <summary>
	/// Sift Implementation
	/// </summary>
	class UnpackedSIFT {

	public:

		/// <summary>
		/// Initializes a new instance of the <see cref="UnpackedSIFT"/> class.
		/// </summary>
		UnpackedSIFT();

		/// <summary>
		/// Initializes a new instance of the <see cref="UnpackedSIFT"/> class.
		/// </summary>
		/// <param name="_nfeatures">The n features.</param>
		/// <param name="_nOctaveLayers">The n octave layers.</param>
		/// <param name="_contrastThreshold">The contrast threshold.</param>
		/// <param name="_edgeThreshold">The edge threshold.</param>
		/// <param name="_sigma">The sigma.</param>
		UnpackedSIFT(int _nfeatures, int _nOctaveLayers, double _contrastThreshold, double _edgeThreshold, double _sigma);


		/// <summary>
		/// returns the descriptor size.
		/// </summary>
		/// <returns></returns>
		int descriptorSize() const;

		/// <summary>
		/// returns the descriptor type.
		/// </summary>
		/// <returns></returns>
		int descriptorType() const;

		/// <summary>
		/// Use sift on the image
		/// </summary>
		/// <param name="_image">The image.</param>
		/// <param name="_mask">The mask.</param>
		/// <param name="keypoints">The keypoints.</param>
		void operator()(cv::InputArray _image, cv::InputArray _mask, std::vector<cv::KeyPoint>& keypoints) const;

		/// <summary>
		/// Use sift on the image
		/// </summary>
		/// <param name="_image">The image.</param>
		/// <param name="_mask">The mask.</param>
		/// <param name="keypoints">The keypoints.</param>
		/// <param name="_descriptors">The descriptors.</param>
		/// <param name="useProvidedKeypoints">if set to <c>true</c> [use provided keypoints].</param>
		void operator()(cv::InputArray _image, cv::InputArray _mask, std::vector<cv::KeyPoint>& keypoints, cv::OutputArray _descriptors, bool useProvidedKeypoints) const;

		/// <summary>
		/// detect keypoints
		/// </summary>
		/// <param name="image">The image.</param>
		/// <param name="keypoints">The keypoints.</param>
		/// <param name="mask">The mask.</param>
		void detectImpl(const cv::Mat & image, std::vector<cv::KeyPoint>& keypoints, const cv::Mat & mask) const;

		/// <summary>
		/// compute descriptors
		/// </summary>
		/// <param name="image">The image.</param>
		/// <param name="keypoints">The keypoints.</param>
		/// <param name="descriptors">The descriptors.</param>
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


		/// <summary>
		/// Finds the scale space extrema.
		/// </summary>
		/// <param name="gauss_pyr">The gauss pyr.</param>
		/// <param name="dog_pyr">The dog pyr.</param>
		/// <param name="keypoints">The keypoints.</param>
		void findScaleSpaceExtrema(const std::vector<cv::Mat>& gauss_pyr, const std::vector<cv::Mat>& dog_pyr, std::vector<cv::KeyPoint>& keypoints) const;

		/// <summary>
		/// Builds the gaussian pyramid.
		/// </summary>
		/// <param name="base">The base.</param>
		/// <param name="pyr">The pyr.</param>
		/// <param name="nOctaves">The n octaves.</param>
		void buildGaussianPyramid(const cv::Mat & base, std::vector<cv::Mat>& pyr, int nOctaves) const;

		/// <summary>
		/// Builds the difference of gaussiang pyramid.
		/// </summary>
		/// <param name="gpyr">The gpyr.</param>
		/// <param name="dogpyr">The dogpyr.</param>
		void buildDoGPyramid(const std::vector<cv::Mat>& gpyr, std::vector<cv::Mat>& dogpyr) const;

	};
}

#endif // !U_SIFT_H